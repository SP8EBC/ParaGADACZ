/*
 * main.cpp
 *
 *  Created on: Nov 28, 2022
 *      Author: mateusz
 */

#include "main.h"
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <vlc/vlc.h>
#include <condition_variable>
#include <list>
#include <vector>
#include <memory>
#include <optional>
#include <algorithm>

// api
#include "ApiConfiguration.h"
#include "ApiClient.h"
#include "ListOfAllStationsApi.h"
#include "StationApi.h"

// meteoblue api
#include "ForecastApi.h"
#include "secret.h"

#include "ForecastFinder.h"
#include "PlaylistAssembler.h"
#include "PlaylistSamplerPL.h"
#include "AprxLogParser.h"
#include "AprsWXDataFactory.h"
#include "ForecastDownloader.h"
#include "Player.h"
#include "InhibitorAndPttControl.h"
#include "AvalancheWarnings.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <string_view>
#include "CurrentConditionsDownloader.h"


//constexpr std::string_view pogoda_base_url = "http://pogoda.cc:8080/meteo_backend_web/";
const utility::string_t pogoda_base_url = "http://pogoda.cc:8080/meteo_backend_web/";
const utility::string_t meteoblue_base_url = "http://my.meteoblue.com/packages/";

//!< Instance of configuration file
std::shared_ptr<ConfigurationFile> configurationFile;

//!< Configuration of data sources for current weather conditions
std::shared_ptr<std::vector<ConfigurationFile_CurrentWeather>> currentWeatherConfig;

//!< Instance of API client used to contact REST API (used only for pogoda.cc meteo_backend)
std::shared_ptr<org::openapitools::client::api::ApiClient> apiClient;

//!< Instance of API configuration class (used only for pogoda.cc meteo_backend)
std::shared_ptr<org::openapitools::client::api::ApiConfiguration> apiConfiguration;

//!< Gets a list of all station
std::shared_ptr<org::openapitools::client::api::ListOfAllStationsApi> listofAllStationApi;

//!< Gets summary for given station and few more things
std::shared_ptr<org::openapitools::client::api::StationApi> stationApi;

//!< List of all station from pogoda.cc meteo_backend
std::vector<std::shared_ptr<org::openapitools::client::model::StationDefinitionModel>> listOfAllStationsPogodacc;

//!< Parser of APRX rf-log file
AprxLogParser logParser;

//!< Instance of media player wrapper
Player player;

//!< Controls PTT line using serial port and optionally inhibits
InhibitorAndPttControl inhibitAndPtt;

//!<
AvalancheWarnings avalancheWarning;

//!< Vector of current weather conditions (only from APRX rf log file
std::vector<AprsWXData> currentWeatherAprx;

//!<
std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> currentWeatherMeteobackend;

//!< Value of Regional pressure (if it is configured)
std::optional<float> regionalPressure;

//!< Interface to meteoblue auto generated code
std::shared_ptr<ForecastDownloader> forecastDownloader;

//!< Returns path or a vector of paths (std::string's) to audio files representing given number or other element
std::shared_ptr<PlaylistSampler> playlistSampler;

//!< Assembly complete playlist
std::shared_ptr<PlaylistAssembler> playlistAssembler;

int main(int argc, char **argv) {

	int downloadParseResult = 0;

	std::string configFn;

	spdlog::set_level(spdlog::level::debug);

	SPDLOG_INFO("============ ParaGADACZ is starting =============");
	SPDLOG_INFO("===== Mateusz Lubecki, Bielsko - Biała 2023 =====");
	SPDLOG_INFO("=================================================");

	if (argc > 1) {
		configFn = std::string(argv[1]);
	}
	else {
		configFn = "paragadacz.conf";
	}

	SPDLOG_INFO("Using configuration file: {}", configFn);

	// create an instance of configuration file
	configurationFile = std::make_shared<ConfigurationFile>(configFn);

	// try parse the configuration file
	const bool configParsingResult = configurationFile->parse();

	// check parsing result
	if (!configParsingResult) {
		SPDLOG_ERROR("Configuration file hasn't been parsed correctly! Exiting application!");

		return -1;
	}

	if (!configurationFile->isDebug()) {
		spdlog::set_level(spdlog::level::info);
	}

	auto inhibitorConfig = configurationFile->getInhibitor();

	// check inhibit conditions by executing configured script / executable
	if (!InhibitorAndPttControl::checkInhibitExec(inhibitorConfig)) {
		SPDLOG_ERROR("Transmission inhibited by EXEC");

		return -10;
	}

	// the same by ask configured HTTP server if we are free to transmit
	if (!InhibitorAndPttControl::checkInhibitHttp(inhibitorConfig)) {
		SPDLOG_ERROR("Transmission inhibited by HTTP server response");

		return -11;
	}

	if (!InhibitorAndPttControl::checkInhibitSerial(inhibitorConfig)) {
		SPDLOG_ERROR("Transmission inhibited by serial port CTS line");

		return -12;
	}

	// create an instance of API client
	apiClient = std::make_shared<org::openapitools::client::api::ApiClient>();

	// create an instance of API configuration
	apiConfiguration = std::make_shared<org::openapitools::client::api::ApiConfiguration>();

	// set base URL
	apiConfiguration->setBaseUrl(pogoda_base_url);

	// set this configuration or API client
	apiClient->setConfiguration(apiConfiguration);

	listofAllStationApi = std::make_shared<org::openapitools::client::api::ListOfAllStationsApi>(apiClient);
	stationApi = std::make_shared<org::openapitools::client::api::StationApi>(apiClient);

	// create an instance of forecast downloader
	forecastDownloader = std::make_shared<ForecastDownloader>(configurationFile);

	// create an instance of playlist sampler in PL variant
	playlistSampler = std::make_shared<PlaylistSamplerPL>(configurationFile);

	// create playlist assembler instance which will use sampler to create playlist from weather data
	playlistAssembler = std::make_shared<PlaylistAssembler>(playlistSampler, configurationFile);

	// check if meteoblue forecasts are enabled in a configuration file
	if (configurationFile->getForecast().enable) {

		// check if demo mode is enabled or not
		if (configurationFile->getForecast().demo) {
			// create stub to have something
			forecastDownloader->createDemoStub();
		}
		else {
			// download all forecast data
			forecastDownloader->downloadAllMeteoblue();
		}

	}

	// check if there is anything to be downloaded from pogoda.cc backend
	if (configurationFile->isHasPogodacc()) {
		// get list of all stations
		listOfAllStationsPogodacc = listofAllStationApi->listOfAllStationsGet().get();
	}

	// check if there is at least one station to be parsed from APRX rf log file
	if (configurationFile->isHasAprx()) {
		// set path to APRX file
		logParser.setFileName(configurationFile->getAprxRfLogPath());

		// open a file
		logParser.openFile();

		// and fast forward it to the middle
		logParser.rewindFile();
	}

	// get configuration for current weather conditions. this is mandatory and is always enabled
	currentWeatherConfig = std::make_shared<std::vector<ConfigurationFile_CurrentWeather>>(configurationFile->getCurrent());

	// download all information about current conditions from Pogoda.cc and/or APRX rf-log using parsed configuration file
	downloadParseResult = CurrentConditionsDownloader::downloadParseCurrentCondotions(
								currentWeatherConfig,
								currentWeatherAprx,
								currentWeatherMeteobackend,
								listOfAllStationsPogodacc,
								stationApi,
								regionalPressure,
								logParser);

	// exit on any error
	if (downloadParseResult != 0) {
		SPDLOG_ERROR("Exiting application! downloadParseResult: {}", downloadParseResult);

		return downloadParseResult;
	}

	// start to create playlist
	playlistAssembler->start();

	// append pre announcement
	playlistAssembler->recordedAnnouncement(false);

	// if there is regional pressure to say
	if (regionalPressure.has_value()) {
		playlistAssembler->regionalPressure(*regionalPressure);
	}

	// insert current weather
	playlistAssembler->currentWeather(currentWeatherMeteobackend, currentWeatherAprx);

	// insert weather forecast
	if (configurationFile->getForecast().enable) {
		if (forecastDownloader->isAnyGood()) {
			if (forecastDownloader->isAnyError() && configurationFile->getForecast().skipAnouncementIfAnyIsMissing) {
				SPDLOG_WARN("Weather forecast announcement skipped due to problem with downloading at least one of them");
			}
			else {
				playlistAssembler->forecastMeteoblue(forecastDownloader->getAllForecast());
			}
		}
		else {
			SPDLOG_WARN("Weather forecast announcement skipped because no forecast data have been downloaded successfully");
		}
	}

	// put avalance warnings
	CurrentConditionsDownloader::downloadParseAvalancheWarnings(
			configurationFile->getAvalancheWarning(),
			avalancheWarning,
			playlistAssembler);

	// put post anouncements
	playlistAssembler->recordedAnnouncement(true);

	// add signoff
	playlistAssembler->signOff();

	// get finished playlist
	auto playlist = playlistAssembler->getPlaylist();

	// print all playlist elements
	if (configurationFile->isDebug()) {
		SPDLOG_INFO("Generated playlist has {} elements", playlist->size());

		for (std::string elem : *playlist) {
			SPDLOG_INFO("Element: {}", elem);
		}
	}

	inhibitAndPtt.setConfigAndCheckPort(configurationFile);

	// key ptt
	inhibitAndPtt.keyPtt(true);

	// set playlist
	player.setPlaylist(playlist, configurationFile->getAudioBasePath());

	// play all files
	while (player.playNext()) {
		player.waitForPlaybackToFinish();
	}

	// dekey PTT
	inhibitAndPtt.dekeyPtt();


	return 0;
}




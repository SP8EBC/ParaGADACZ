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

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <string_view>


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

//!< Vector of current weather conditions (only from APRX rf log file
std::vector<AprsWXData> currentWeatherAprx;

//!<
std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> currentWeatherMeteobackend;

//!< Value of Regional pressure (if it is configured)
std::optional<float> regionalPressure;

//!< Interface to meteoblue auto generated code
std::shared_ptr<ForecastDownloader> forecastDownloader;

std::shared_ptr<PlaylistSampler> playlistSampler;

std::shared_ptr<PlaylistAssembler> playlistAssembler;

int main(int argc, char **argv) {

	std::string configFn;

	spdlog::set_level(spdlog::level::debug);

	SPDLOG_INFO("ParaGADACZ is starting");

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

	playlistAssembler = std::make_shared<PlaylistAssembler>(playlistSampler, configurationFile);

	// check if meteoblue forecasts are enabled in a configuration file
	if (configurationFile->getForecast().enable) {
		// download all forecast data
		forecastDownloader->downloadAllMeteoblue();
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

	// go through configuration and download current weather conditions
	for (ConfigurationFile_CurrentWeather current : *currentWeatherConfig) {
		switch (current.type) {
		case APRX: {
			// split callsign from SSID
			std::tuple<std::string, unsigned> split = ConfigurationFile::splitCallsign(current.name);

			// get last packet for that station
			std::optional<AprsWXData> packet = logParser.getLastPacketForStation(std::get<std::string>(split), std::get<unsigned>(split));

			if (packet.has_value()) {
				// set what should be used from this packet
				packet->useWind = current.sayWind;
				packet->useTemperature = current.sayTemperature;
				packet->useHumidity = current.sayHumidy;
				packet->usePressure = current.sayPressure;

				// check if this station should provide regional pressure value
				if (current.regionalPressure) {

					// check if regional pressure was get before
					if (regionalPressure.has_value()) {
						SPDLOG_ERROR("More than one source of regional pressure found!");

						return -2;
					}
					else {
						regionalPressure = packet->pressure;
					}
				}

				SPDLOG_INFO("Adding current weather conditions from APRX for station: {}", current.name);

				// add this to the vector of current weather
				currentWeatherAprx.push_back(std::move(*packet));
			}
			else {
				SPDLOG_ERROR("Cannot find any current conditions for {} within APRX rf log file!!", current.name);
#ifdef MAIN_FAIL_ON_MISSING_CURRENT_CONDITIONS
				return -3;
#else
#endif
			}

			break;
		}

		case POGODA_CC: {
			// look for weather station in results from meteo_backend
			auto forecast = std::find_if(listOfAllStationsPogodacc.begin(), listOfAllStationsPogodacc.end(), [& current](auto x) {
				if (x->getName() == current.name) {
					return true;
				}
				else {
					return false;
				}
			});

			// check if this station was found or not
			if (forecast != listOfAllStationsPogodacc.end()) {
				SPDLOG_INFO("Downloading meteo_backend summary for {}", (*forecast)->getName());

				// get summary for that station
				std::shared_ptr<org::openapitools::client::model::Summary> summary = stationApi->stationStationNameSummaryGet((*forecast)->getName()).get();

				SPDLOG_INFO("Adding current weather conditions from meteo_backend for station: {}", current.name);

				// add this result to
				currentWeatherMeteobackend.push_back({current.name, summary});

			}
			else {
				SPDLOG_ERROR("Cannot find any current conditions for {} within list of all station from pogoda.cc meteo_backend!!", current.name);
#ifdef MAIN_FAIL_ON_MISSING_CURRENT_CONDITIONS
				return -4;
#else
#endif
			}
			break;
		}
		default:
			SPDLOG_ERROR("Unsupported current weather source");
		}
	}

	// start to create playlist
	playlistAssembler->start();

	// append pre announcement
	playlistAssembler->recordedAnnouncement(false);

	// if there is regional pressure to say
	if (regionalPressure.has_value()) {
		playlistAssembler->regionalPressure(*regionalPressure);
	}

	// inset current weather
	playlistAssembler->currentWeather(currentWeatherMeteobackend, currentWeatherAprx);

	playlistAssembler->forecastMeteoblue(forecastDownloader->getAllForecast());

	return 0;
}




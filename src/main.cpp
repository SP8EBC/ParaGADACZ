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

// meteoblue api
#include "ForecastApi.h"
#include "secret.h"

#include "ForecastFinder.h"
#include "PlaylistAssembler.h"
#include "PlaylistSamplerPL.h"
#include "AprxLogParser.h"
#include "AprsWXDataFactory.h"
#include "ForecastDownloader.h"
#include "PogodaccDownloader.h"
#include "WeatherlinkDownloader.h"
#include "Player.h"
#include "InhibitorAndPttControl.h"
#include "AvalancheWarnings.h"
#include "TrendDownloader.h"
#include "EmailDownloader.h"
#include "SpeechSynthesis.h"
#include "TimeTools.h"
#include "./exception/NoEmailsToSayEx.h"

#ifdef PANSA_AIRSPACE_ENABLED
#include "PansaAirspace.h"
#include "PlaylistAssemblerAirspace.h"
#endif

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

#include <string_view>
#include "CurrentConditionsDownloader.h"

//constexpr std::string_view pogoda_base_url = "http://pogoda.cc:8080/meteo_backend_web/";
const utility::string_t pogoda_base_url = "http://pogoda.cc:8080/meteo_backend_web/";
const utility::string_t meteoblue_base_url = "http://my.meteoblue.com/packages/";
const utility::string_t weatherlink_base_url = "https://api.weatherlink.com/v1/";

//!< Instance of configuration file
std::shared_ptr<ConfigurationFile> configurationFile;

//!< Configuration of data sources for current weather conditions
std::shared_ptr<std::vector<ConfigurationFile_CurrentWeather>> currentWeatherConfig;

//!< Used to download list of all stations and current conditions summary from Pogoda.cc
std::shared_ptr<PogodaccDownloader> pogodaccDownloader;

//!< Parser of APRX rf-log file
AprxLogParser logParser;

//!< Instance of media player wrapper
Player player;

//!< Controls PTT line using serial port and optionally inhibits
InhibitorAndPttControl inhibitAndPtt;

//!< Scraps avalanche warnings from GOPR website
AvalancheWarnings avalancheWarning;

//!< Vector of current weather conditions (only from APRX rf log file
std::vector<AprsWXData> currentWeatherAprx;

//!< Vector of current conditions downloaded from WeatherLink v1 API provided by Davis
std::vector<std::tuple<std::string, AprsWXData>> currentWeatherDavisWeatherlink;

//!<
std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> currentWeatherMeteobackend;

//!< Value of Regional pressure (if it is configured)
std::optional<float> regionalPressure;

//!< Interface to meteoblue auto generated code
std::shared_ptr<ForecastDownloader> forecastDownloader;

//!< Weatherlink downloader
std::shared_ptr<WeatherlinkDownloader> weatherlinkDownloader;

//!< Returns path or a vector of paths (std::string's) to audio files representing given number or other element
std::shared_ptr<PlaylistSampler> playlistSampler;

//!< Assembly complete playlist
std::shared_ptr<PlaylistAssembler> playlistAssembler;

//!< Vector to hold trend data obtained by TrendDownloader
std::vector<TrendDownloader_Data> trend;

std::shared_ptr<EmailDownloader> emailDownloader;

std::vector<EmailDownloaderMessage> validatedEmails;

std::shared_ptr<SpeechSynthesis> speechSyntesis;

#ifdef PANSA_AIRSPACE_ENABLED
std::shared_ptr<PansaAirspace> pansa;

std::shared_ptr<PlaylistAssemblerAirspace> playlistAssemblerAirspace;//(playlist_sampler, config_fixed_anouncement_dictionary_sayalt_saytime);

bool bailoutCheck(	const bool isLogicalAndOnBailout,
					const ConfigurationFile_Airspace& airspace,
					const ConfigurationFile_Email& emailAnnonuncements,
					const ConfigurationFile_SpeechSynthesis& speechSynthesis,
					bool isHasSomethingToSay,
					int ttsAnouncmtsFromEmails)
{

	bool bailoutCondition = false;

	if (isLogicalAndOnBailout) {
		if (airspace.enabled && airspace.bailoutIfNothingToSay) {
			bailoutCondition = !playlistAssemblerAirspace->isHasSomethingToSay();
		}
		else {
			bailoutCondition = true;
		}

		if (emailAnnonuncements.enabled && speechSynthesis.bailoutIfNoMailsToSay) {
			bailoutCondition = bailoutCondition & (ttsAnouncmtsFromEmails == 0);
		}

	}
	else {
		if (airspace.enabled && airspace.bailoutIfNothingToSay) {
			bailoutCondition = !playlistAssemblerAirspace->isHasSomethingToSay();
		}
		if (bailoutCondition == false && emailAnnonuncements.enabled && speechSynthesis.bailoutIfNoMailsToSay) {
			bailoutCondition = (ttsAnouncmtsFromEmails == 0);
		}
	}

	if (bailoutCondition) {
		if (!playlistAssemblerAirspace->isHasSomethingToSay()) {
			SPDLOG_ERROR("No airspace restriction announcements have been generated, due to configuration");
			SPDLOG_ERROR("and/or because no reservations are currently active in todays AUP.");
		}

		if ((ttsAnouncmtsFromEmails == 0)) {
			SPDLOG_ERROR("There is also no text-to-speech announcements from Emails.");
		}

		SPDLOG_ERROR("Program will exit because BailoutIfNothingToSay is enabled.");
	}

	return bailoutCondition;

}
#endif

#ifdef __linux__
#include <signal.h>

void intHandler(int dummy) {
	// dekey PTT
	inhibitAndPtt.dekeyPtt();

	exit(-1);
}
#endif

int main(int argc, char **argv) {

	int downloadParseResult = 0;

	int ttsAnouncmtsFromEmails = 0;

	std::string configFn;

	spdlog::set_level(spdlog::level::debug);

	SPDLOG_INFO("============ ParaGADACZ is starting =============");
	SPDLOG_INFO("===== Mateusz Lubecki, Bielsko - Biała 2024 =====");
	SPDLOG_INFO("=================================================");
	SPDLOG_INFO("Application version: {}, from: {}", MAIN_SOFTWARE_VERSION, MAIN_SOFTWARE_DATE);


	SPDLOG_INFO("Application startup UTC time: {}", boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time()));


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

	const bool hasCurrentConditions = configurationFile->isHasPogodacc() ||
			configurationFile->isHasAprx() || configurationFile->isHasWeatherlink();

	// check parsing result
	if (!configParsingResult) {
		SPDLOG_ERROR("Configuration file hasn't been parsed correctly! Exiting application!");

		return -1;
	}

	if (!configurationFile->isDebug()) {
		spdlog::set_level(spdlog::level::info);
	}

	if (configurationFile->isOnlyPttDekey()) {
		SPDLOG_INFO("Only dekeying PTT");
		inhibitAndPtt.setConfigAndCheckPort(configurationFile);

		inhibitAndPtt.dekeyPtt();

		return 0;
	}

	TimeTools::initBoostTimezones(configurationFile->getZoneSpecificationFilePath());

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

	pogodaccDownloader = std::make_shared<PogodaccDownloader>(configurationFile);

	// create an instance of forecast downloader
	forecastDownloader = std::make_shared<ForecastDownloader>(configurationFile);

	// create an instance for download weatherlink
	weatherlinkDownloader = std::make_shared<WeatherlinkDownloader>(configurationFile);

	// create an instance of playlist sampler in PL variant
	playlistSampler = std::make_shared<PlaylistSamplerPL>(configurationFile);

	// create playlist assembler instance which will use sampler to create playlist from weather data
	playlistAssembler = std::make_shared<PlaylistAssembler>(playlistSampler, configurationFile);

	emailDownloader = std::make_shared<EmailDownloader>(configurationFile->getEmailAnnonuncements());

	speechSyntesis = std::make_shared<SpeechSynthesisResponsivevoice>(configurationFile->getSecrets().responsiveVoiceApiKey,
																		configurationFile->getSpeechSynthesis().pitch,
																		configurationFile->getSpeechSynthesis().rate,
																		configurationFile->getSpeechSynthesis().maximumTimeout);

#ifdef PANSA_AIRSPACE_ENABLED
	playlistAssemblerAirspace = std::make_shared<PlaylistAssemblerAirspace>(
												playlistSampler, configurationFile);

	pansa = std::make_shared<PansaAirspace>(
							configurationFile->getAirspace().postgresUsername,
							configurationFile->getAirspace().postgresPassword,
							configurationFile->getAirspace().postgresDb);
#endif

	if (configurationFile->getEmailAnnonuncements().enabled) {
		emailDownloader->downloadAllEmailsImap();

		emailDownloader->validateEmailAgainstPrivileges();

		emailDownloader->copyOnlyValidatedEmails(validatedEmails);

		emailDownloader->checkEmailConfig(configFn);
	}

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
		pogodaccDownloader->downloadAllStationsList();
	}

	// check if there is at least one station to be parsed from APRX rf log file
	if (configurationFile->isHasAprx()) {
		// set path to APRX file
		logParser.setFileName(configurationFile->getAprxConfig().aprxRfLogPath);

		// open a file
		logParser.openFile();

		// and fast forward it to the middle
		logParser.rewindFile();

		logParser.setTimestampsAreInLocal(configurationFile->getAprxConfig().aprxRfLogTimeInLocal);
	}

	// get configuration for current weather conditions. this is mandatory and is always enabled
	currentWeatherConfig = std::make_shared<std::vector<ConfigurationFile_CurrentWeather>>(configurationFile->getCurrent());

	// download all information about current conditions from Pogoda.cc and/or APRX rf-log using parsed configuration file
	downloadParseResult = CurrentConditionsDownloader::downloadParseCurrentCondotions(
								currentWeatherConfig,
								currentWeatherAprx,
								currentWeatherDavisWeatherlink,
								currentWeatherMeteobackend,
								pogodaccDownloader->getListOfAllStationsPogodacc(),
								pogodaccDownloader->getStationApi(),
								regionalPressure,
								logParser,
								weatherlinkDownloader);

	// download trend data (a check if it is even enabled is done inside this method)
	TrendDownloader::downloadTrendData(trend, *configurationFile, pogodaccDownloader->getStationApi(), logParser, TimeTools::getEpoch());

	// exit on any error
	if (downloadParseResult != 0) {
		SPDLOG_ERROR("Exiting application! downloadParseResult: {}", downloadParseResult);

		return downloadParseResult;
	}

#ifdef PANSA_AIRSPACE_ENABLED

	const bool airspaceDumpSql = configurationFile->getAirspace().dumpSqlQueries;

	if (configurationFile->getAirspace().enabled) {
		// iterate through all configured points and download all active airspace activation for them
		for (ConfigurationFile_Airspace_AroundPoint apoint : configurationFile->getAirspace().aroundPoint) {
			pansa->downloadAroundLocation(apoint.audioFilename, apoint.latitude, apoint.longitude, apoint.radius, airspaceDumpSql);
		}

		// iterate through all fixed-conigured airspaces and check if there are any reservations for them
		for (ConfigurationFile_Airspace_Fixed fx : configurationFile->getAirspace().fixed) {
			pansa->downloadForDesginator(fx.designator, fx.sayAltitudes, fx.sayTimes, airspaceDumpSql);
		}
	}
#endif

	// start to create playlist
	playlistAssembler->start();

	// append pre announcement
	playlistAssembler->recordedAnnouncement(false);

	if (configurationFile->getEmailAnnonuncements().enabled && configurationFile->getSpeechSynthesis().placeAtTheEnd == false) {
		const PlaylistAssembler_TextToSpeechAnnouncement_Stats stats = playlistAssembler->textToSpeechAnnouncements(validatedEmails);
		ttsAnouncmtsFromEmails = stats.added;
	}

	// if there is regional pressure to say
	if (regionalPressure.has_value()) {
		playlistAssembler->regionalPressure(*regionalPressure);
	}

	if (hasCurrentConditions) {
		// insert current weather
		playlistAssembler->currentWeather(currentWeatherMeteobackend, currentWeatherAprx, currentWeatherDavisWeatherlink, std::optional<std::vector<TrendDownloader_Data>>(std::move(trend)));
	}

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

#ifdef PANSA_AIRSPACE_ENABLED
	playlistAssemblerAirspace->setPlaylist(playlistAssembler->getPlaylist());

	const std::vector<std::shared_ptr<PansaAirspace_ResultsAroundPoint>> & aroundPoints = pansa->getReservationsAroundPoints();

	const std::vector<std::shared_ptr<PansaAirspace_ResultsForDesignator> >& fixedZones = pansa->getReservationsForZones();

	for (const std::shared_ptr<PansaAirspace_ResultsForDesignator> & zone : fixedZones) {
		playlistAssemblerAirspace->reservationsForExplicitlyConfAirspace(zone->designator, zone->sayAltitude, zone->sayTime, zone->type, zone->reservations);
	}

	for (const std::shared_ptr<PansaAirspace_ResultsAroundPoint> & point : aroundPoints) {
		playlistAssemblerAirspace->reservationsAroundPoint(point->radius, point->pointName, point->reservations);
	}

#endif

	// put post anouncements
	playlistAssembler->recordedAnnouncement(true);

	if (configurationFile->getEmailAnnonuncements().enabled && configurationFile->getSpeechSynthesis().placeAtTheEnd == true) {
		const PlaylistAssembler_TextToSpeechAnnouncement_Stats stats = playlistAssembler->textToSpeechAnnouncements(validatedEmails);
		ttsAnouncmtsFromEmails = stats.added;
	}

	// add signoff
	playlistAssembler->signOff();

	// get finished playlist
	auto playlist = playlistAssembler->getPlaylist();

#ifdef PANSA_AIRSPACE_ENABLED
	const bool bailout = bailoutCheck(configurationFile->isLogicalAndOnBailout(),
										configurationFile->getAirspace(),
										configurationFile->getEmailAnnonuncements(),
										configurationFile->getSpeechSynthesis(),
										playlistAssemblerAirspace->isHasSomethingToSay(),
										ttsAnouncmtsFromEmails);

	if (bailout) {
		return 0;
	}
#else
	if (ttsAnouncmtsFromEmails == 0 && configurationFile->getSpeechSynthesis().bailoutIfNoMailsToSay) {
		SPDLOG_ERROR("There is nothing to say from email messages! Program will not continue!");
		throw NoEmailsToSayEx();
	}
#endif



	// print all playlist elements
	if (configurationFile->isDebug()) {
		SPDLOG_INFO("Generated playlist has {} elements", playlist->size());

		for (std::string elem : *playlist) {
			SPDLOG_INFO("Element: {}", elem);
		}
	}

	if (configurationFile->isOnlyAssemblePlaylist()) {
		SPDLOG_WARN("Configured to exit w/o playing anything");
		return 0;
	}

	inhibitAndPtt.setConfigAndCheckPort(configurationFile);

#ifdef __linux__
    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);
#endif

	try {
		// key ptt
		inhibitAndPtt.keyPtt(true);
	}
	catch (const std::runtime_error & e) {
		SPDLOG_ERROR(e.what());

		return -20;
	}

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




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
#include <memory>

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

//!< Instance of API client used to contact REST API (used only for meteo_backend)
std::shared_ptr<org::openapitools::client::api::ApiClient> apiClient;

//!< Instance of API configuration class (used only for meteo_backend)
std::shared_ptr<org::openapitools::client::api::ApiConfiguration> apiConfiguration;

//!< Gets a list of all station
std::shared_ptr<org::openapitools::client::api::ListOfAllStationsApi> listofAllStation;

//!< Gets summary for given station and few more things
std::shared_ptr<org::openapitools::client::api::StationApi> stationApi;

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

    //auto type = listofAllStation.listOfAllStationsGet().get();

	// get configuration for current weather conditions. this is mandatory and is always enabled
	currentWeatherConfig = std::make_shared<std::vector<ConfigurationFile_CurrentWeather>>(configurationFile->getCurrent());

	for (ConfigurationFile_CurrentWeather current : *currentWeatherConfig) {
		switch (current.type) {
		case APRX:
		case POGODA_CC:
		default:
			SPDLOG_ERROR("Unsupported current weather source");
		}
	}

	return 0;
}




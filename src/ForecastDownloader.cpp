/*
 * ForecastDownloader.cpp
 *
 *  Created on: Jan 16, 2023
 *      Author: mateusz
 */

#include "ForecastDownloader.h"

// meteoblue api
#include "ForecastApi.h"
#include "secret.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

ForecastDownloader::ForecastDownloader(ConfigurationFile & config) : configurationFile(config) {

	apiClient = std::make_shared<org::openapitools::client::api::ApiClient>();
	apiConfiguration.setBaseUrl("http://pogoda.cc:8080/meteo_backend_web/");
	auto apiConfigurationPtr = std::shared_ptr<org::openapitools::client::api::ApiConfiguration>(&apiConfiguration);

	forecastApi = std::make_shared<org::openapitools::client::api::ForecastApi>(apiClient);
}

ForecastDownloader::~ForecastDownloader() {
	// TODO Auto-generated destructor stub
}

ForecastDownloader::ForecastDownloader(const ForecastDownloader &other) : configurationFile(other.configurationFile) {
	// TODO Auto-generated constructor stub

}

ForecastDownloader::ForecastDownloader(ForecastDownloader &&other) : configurationFile(other.configurationFile) {
	// TODO Auto-generated constructor stub

}

ForecastDownloader& ForecastDownloader::operator=(
		const ForecastDownloader &other) {
	// TODO Auto-generated method stub

}

ForecastDownloader& ForecastDownloader::operator=(ForecastDownloader &&other) {
	// TODO Auto-generated method stub

}

void ForecastDownloader::downloadAllMeteoblue() {

	// get all configured forecast points
	const ConfigurationFile_ForecastMeteoblue & forecasts = configurationFile.getForecast();

	// iterate through all locations
	for (ConfigurationFile_ForecastMeteoblue_Locations location : forecasts.locations) {

		SPDLOG_INFO("Downloading meteoblue forecast for: {}", location.name);

		// download forecast (at least try to do so)
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> forecast =
				forecastApi->basicDayBasic3hGet(
						location.longitude,
						location.latitude,
						"timestamp_utc",
						METEOBLUE_API_KEY,
						boost::optional<std::string>()).get();

		// put result
		std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>> tuple;

		// create a tuple with results
		tuple = std::make_tuple(location.name, forecast);

		// add this to the vector
		allResults.push_back(tuple);
	}


	/**
	 *
	 * forecastApi.basicDayBasic3hGet(19.03, 49.68, "timestamp_utc", METEOBLUE_API_KEY, boost::optional<std::string>()).get();
	 */
}

std::shared_ptr<org::openapitools::client::model::Inline_response_200> ForecastDownloader::getForName(
		std::string forecastPointName) {
}

std::vector<
		std::tuple<std::string,
				org::openapitools::client::model::Inline_response_200> >& ForecastDownloader::getAllForecast() {
}
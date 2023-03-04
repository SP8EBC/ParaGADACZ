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

ForecastDownloader::ForecastDownloader(std::shared_ptr<ConfigurationFile> & config) : configurationFile(config) {

	apiConfiguration = std::make_shared<org::openapitools::client::api::ApiConfiguration>();
	apiConfiguration->setBaseUrl("http://my.meteoblue.com/packages/");

	apiClient = std::make_shared<org::openapitools::client::api::ApiClient>();
	apiClient->setConfiguration(apiConfiguration);

	forecastApi = std::make_shared<org::openapitools::client::api::ForecastApi>(apiClient);
}

ForecastDownloader::~ForecastDownloader() {
	// TODO Auto-generated destructor stub
}

bool ForecastDownloader::downloadAllMeteoblue() {

	// get all configured forecast points
	const ConfigurationFile_ForecastMeteoblue & forecasts = configurationFile->getForecast();

	// iterate through all locations
	for (ConfigurationFile_ForecastMeteoblue_Locations location : forecasts.locations) {

		SPDLOG_INFO("Downloading meteoblue forecast for: {}, longitude: {}, latitude: {}", location.name, location.longitude, location.latitude);

		try {
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

			anyGood = true;
		}
		catch (org::openapitools::client::api::ApiException & e) {
			SPDLOG_ERROR("ApiException has happened during downloading forecast for {}", location.name );

			anyError = true;
		}
		catch (...) {
			SPDLOG_ERROR("Something really wrong has happened during downloading forecast for {}", location.name );

			anyError = true;
		}
	}

	return anyError;

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
		std::shared_ptr<org::openapitools::client::model::Inline_response_200>> > & ForecastDownloader::getAllForecast() {

	return allResults;
}

/**
 * ONLY FOR DEMO PURPOSES!!!!
 */
bool ForecastDownloader::createDemoStub() {
#define START_TEMPERATURE	-2.0f
#define START_WINDSPEED		6.0f
#define START_WINDDIRECTION	230

	SPDLOG_ERROR("WARNING!!! FORECAST DEMO MODE ENABLED!! Stubbing it.");

	// get currrent timestamp
	boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

	// epoch
	boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

	// timestamp
	long ts = (current - epoch).total_seconds();

	// forecast future time in minutes!!!
	uint32_t future_time = configurationFile->getForecast().futureTime;

	//org::openapitools::client::model::Inline_response_200
	auto meteoblueResponse = std::make_shared<org::openapitools::client::model::Inline_response_200>();
	auto data3h = std::make_shared<org::openapitools::client::model::Data_3h>();

	std::vector<int32_t> time;
	std::vector<float> temperature;
	std::vector<int32_t> winddirection;
	std::vector<float> windspeed;

	int64_t expected_timestamp = ts + (future_time / 30) * 1800;

	////////////// prepare stubbed weather forecast data
	for (int i = 0; i < 64; i++) {
		time.push_back(ts + i * 1800);
		temperature.push_back(START_TEMPERATURE + 0.1f * i);
		windspeed.push_back(START_WINDSPEED + 0.1f * i);
		winddirection.push_back(START_WINDDIRECTION + i);
	}


	data3h->setTime(time);
	data3h->setTemperature(temperature);
	data3h->setWinddirection(winddirection);
	data3h->setWindspeed(windspeed);

	meteoblueResponse->setData3h(data3h);

	std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>> skrzyczne = std::make_tuple("skrzyczne", meteoblueResponse);
	std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>> jezioro = std::make_tuple("jezioro", meteoblueResponse);

	allResults.push_back(skrzyczne);
	allResults.push_back(jezioro);

	anyGood = true;

	return true;

}

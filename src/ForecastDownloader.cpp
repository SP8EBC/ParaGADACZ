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

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <map>
#include <utility>

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

		// used to extract from json object
		std::stringstream stream;

		try {
			// download forecast (at least try to do so)
			std::shared_ptr<org::openapitools::client::model::Inline_response_200> forecast =
					forecastApi->basicDayBasic3hGet(
							location.longitude,
							location.latitude,
							"timestamp_utc",
							METEOBLUE_API_KEY,
							boost::optional<std::string>()).get();

			// get response as json object
			web::json::value forecastInJsonValue = forecast->toJson();

			// stream JSON object as a string
			stream << forecastInJsonValue;

			// and get this tring from
			std::string str = stream.str();

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

bool ForecastDownloader::saveInCache(std::string &data, std::string &name) {
}

std::shared_ptr<org::openapitools::client::model::Inline_response_200> ForecastDownloader::loadFromCache(
		std::string name) {
}

bool ForecastDownloader::loadCacheIndex() {

	bool out = false;

	// input buffer from data read from string
	std::string inputBuffer;

	// path to remporary directory
	boost::filesystem::path dir(this->configurationFile->getForecast().cacheDirectory);

	// path to cache index in temporary directory
	boost::filesystem::path index(this->configurationFile->getForecast().cacheDirectory + "/index.json");

	// check if temporary directory exists
	if (boost::filesystem::is_directory(dir)) {
		// check if index file exist
		if (boost::filesystem::exists(index)) {
			SPDLOG_INFO("Opening cache index file from: {}", index.generic_string());

			// open file from disk
			std::fstream inputFile;
			inputFile.open(index.generic_string(), std::ios::in);

			// check if has been opened successfully
			if (inputFile.is_open()) {

				// reserve place for whole index file
				inputFile.seekg(0, std::ios::end);
				inputBuffer.resize(inputFile.tellg());

				// and rewind back to the begining
				inputFile.seekg(0);

				// read all content
				inputFile.read(inputBuffer.data(), inputBuffer.size());

				// try to parse JSON read from file
				nlohmann::basic_json json = nlohmann::json::parse(inputBuffer, nullptr, false);

				// check if parsing was successfull
				if (nlohmann::json::value_t::discarded != json) {

					// index is a root label for the content
					if (json.contains("index")) {
						nlohmann::json array = json["index"];

						// get number of elements
						nlohmann::json::size_type elemNumbers = array.size();

						for (unsigned i = 0 ; i < static_cast<unsigned>(elemNumbers); i++) {

							// get element from index
							nlohmann::json elem = array[i];

							// each element shall consist exactly three elements
							if (elem.size() == 3) {

								// get all values from JSON
								long _timestamp = elem["timestamp"];
								std::string _location = elem["locationName"];
								std::string _filename = elem["filename"];

								// create new object to store into the map
								ForecastDownloader_CacheIndexElem elem;

								elem.filename = _filename;
								elem.locationName = _location;
								elem.timestamp = _timestamp;

								// put element on the map
								this->cacheIndex.insert(std::pair{_filename, elem});

								out = true;
							}
							else {
								SPDLOG_DEBUG("Element at index {} has wrong size of {}", i, elem.size());
							}
						}
					}
				}
				else {
					SPDLOG_ERROR("Cache index doesn't contain valid JSON data");
				}

			}
			else {
				SPDLOG_ERROR("Error has happened during opening!!");
			}
		}
		else {
			SPDLOG_WARN("Cache index file doesn't exist. New one will be probably created later.");
		}
	}

	return out;
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

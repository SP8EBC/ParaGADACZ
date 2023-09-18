/*
 * ForecastDownloader.cpp
 *
 *  Created on: Jan 16, 2023
 *      Author: mateusz
 */

#include "ForecastDownloader.h"

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

// meteoblue api
#include "ForecastApi.h"
#include "ModelBase.h"
#include "secret.h"

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <map>
#include <utility>

#include "exception/ForecastInCacheTooOldEx.h"
#include "exception/ForecastMissingInCache.h"
#include "TimeTools.h"

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

	// index loading result
	bool indexResult;

	// check if cache is enabled
	if (forecasts.cache) {
		SPDLOG_INFO("Loading forecast cache index from disk");

		// load index
		indexResult = loadCacheIndex();
	}

	// iterate through all locations
	for (ConfigurationFile_ForecastMeteoblue_Locations location : forecasts.locations) {

		// if index has been loaded
		if (indexResult) {
			try {
				// load it from cache
				const auto forecast = loadFromCache(location.name);

				// create a tuple with results
				const auto tuple = std::make_tuple(location.name, forecast);

				// add this to the vector
				allResults.push_back(tuple);

				anyGood = true;
			}
			catch (ForecastInCacheTooOldEx & e) {
				SPDLOG_WARN(e.message());

				// if forecast cannot be loaded from cache, download this from the API
				downloadMeteoblue(location);
			}
		}
		else {
			downloadMeteoblue(location);
		}

	}

	return anyError;

	/**
	 *
	 * forecastApi.basicDayBasic3hGet(19.03, 49.68, "timestamp_utc", METEOBLUE_API_KEY, boost::optional<std::string>()).get();
	 */
}

bool ForecastDownloader::downloadMeteoblue(
		ConfigurationFile_ForecastMeteoblue_Locations &location) {

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

		this->saveInCache(str, location.name);

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

	return true;
}

std::shared_ptr<org::openapitools::client::model::Inline_response_200> ForecastDownloader::getForName(
		std::string forecastPointName) {
}

std::shared_ptr<org::openapitools::client::model::Inline_response_200> ForecastDownloader::loadFromCache(
		std::string name) {

	std::shared_ptr<org::openapitools::client::model::Inline_response_200> out;

	std::string buffer;

	// look for element in index
	auto idxIterator = this->cacheIndex.find(name);

	// check if element has been found
	if (idxIterator != this->cacheIndex.end()) {

		// get an element itself
		ForecastDownloader_CacheIndexElem elementFromIndex = idxIterator->second;

		const long currentEpochTime = TimeTools::getEpoch();

		// maximum allowed age of weather data in seconds from now
		const long cacheTimestampAgeLimit = this->configurationFile->getForecast().cacheAgeLimit * 60;

		// check if data isn't too old
		if (elementFromIndex.timestamp == 1 ||
			elementFromIndex.timestamp + cacheTimestampAgeLimit > currentEpochTime) {

			// path to cache file
			boost::filesystem::path file(this->configurationFile->getForecast().cacheDirectory + "/" + elementFromIndex.filename);

			// fstream to get content of a file
			std::fstream cacheFile;
			cacheFile.open(file.generic_string(), std::ios::in);

			if (cacheFile.is_open()) {
				SPDLOG_INFO("Loading stored forecast for {} from cache file {}", elementFromIndex.locationName, file.generic_string());

				// reserve place for whole file file
				cacheFile.seekg(0, std::ios::end);
				buffer.resize(cacheFile.tellg());

				// and rewind back to the begining
				cacheFile.seekg(0);

				// read all content
				cacheFile.read(buffer.data(), buffer.size());

				// create output object
				out = std::make_shared<org::openapitools::client::model::Inline_response_200>();

				// convert a string containing JSON data in text to JSON object itself
				web::json::value value = web::json::value::parse(buffer);

				out->fromJson(value);

			}
		}
		else {
			throw ForecastInCacheTooOldEx(name, currentEpochTime - elementFromIndex.timestamp);
		}
	}
	else {
		throw ForecastMissingInCache();
	}

	return out;
}

bool ForecastDownloader::saveInCache(std::string &data, std::string &name) {

	bool output = false;

	// path to remporary directory
	boost::filesystem::path dir(this->configurationFile->getForecast().cacheDirectory);

	// path to file with this forecast
	boost::filesystem::path file(this->configurationFile->getForecast().cacheDirectory + "/" + name + ".json");

	SPDLOG_INFO("Saving meteoblue forecast for location {} in cache file {}", name, file.generic_string());

	// check if temporary directory exists
	if (boost::filesystem::is_directory(dir)) {

		// open file from disk
		std::fstream inputFile;
		inputFile.open(file.generic_string(), std::ios::out | std::ios::trunc);

		if (inputFile.is_open()) {
			// save data to file
			inputFile << data;

			// close it
			inputFile.close();

			ForecastDownloader_CacheIndexElem cacheIndexElement;

			// create new index element
			cacheIndexElement.filename = name + ".json";
			cacheIndexElement.locationName = name;
			cacheIndexElement.timestamp = TimeTools::getEpoch();

			this->cacheIndex.erase(cacheIndexElement.locationName);

			// put element on the map
			this->cacheIndex.insert(std::pair{cacheIndexElement.locationName, cacheIndexElement});

			// update index on disk
			saveCacheIndex();

			output = true;
		}
		else {
			SPDLOG_ERROR("Cannot open cache file for rewriting!!");
		}
	}
	else {
		SPDLOG_ERROR("Directory for cache files doesn't exist!!");
	}

	return output;
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
			SPDLOG_DEBUG("Opening cache index file from: {}", index.generic_string());

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

				inputFile.close();

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
								this->cacheIndex.insert(std::pair{_location, elem});

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
	else {
		SPDLOG_ERROR("Cache directory doesn't exist!");
	}

	return out;
}

bool ForecastDownloader::saveCacheIndex() {

	return saveCacheIndex(this->cacheIndex);

}

bool ForecastDownloader::saveCacheIndex(
		std::map<std::string, ForecastDownloader_CacheIndexElem> &idx)
{
	bool out = false;

	SPDLOG_INFO("Saving forecast cache index to disk");

	// path to remporary directory
	boost::filesystem::path dir(this->configurationFile->getForecast().cacheDirectory);

	// path to cache index in temporary directory
	boost::filesystem::path index(this->configurationFile->getForecast().cacheDirectory + "/index.json");

	// check if temporary directory exists
	if (boost::filesystem::is_directory(dir)) {

		std::fstream indexStream;

		// open a file with an index and truncate it's content
		indexStream.open(index.generic_string(), std::ios::out | std::ios::trunc);

		if (indexStream.is_open()) {

			// root of a document
			nlohmann::json json = nlohmann::json::object();

			// array with index elements
			nlohmann::json arr = nlohmann::json::array();

			for (const auto & [locationName, indexElem] : idx) {
				// create single list object
				nlohmann::basic_json _indexElementJson = nlohmann::json::object();

				// fill it with data
				_indexElementJson["filename"] = (indexElem.timestamp);
				_indexElementJson["locationName"] = (indexElem.locationName);
				_indexElementJson["filename"] = (indexElem.filename);

				// and add to the array
				arr.push_back(_indexElementJson);
			}

			// insert array into JSON structure
			json["index"] = arr;

			// put JSON into file on disk
			indexStream << json;

			indexStream.close();

			out = true;
		}

	}
	else {
		SPDLOG_ERROR("Target directory for cache doesn't exist!");
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
	//uint32_t future_time = configurationFile->getForecast().futureTime;

	//org::openapitools::client::model::Inline_response_200
	auto meteoblueResponse = std::make_shared<org::openapitools::client::model::Inline_response_200>();
	auto data3h = std::make_shared<org::openapitools::client::model::Data_3h>();

	std::vector<int32_t> time;
	std::vector<float> temperature;
	std::vector<int32_t> winddirection;
	std::vector<float> windspeed;

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

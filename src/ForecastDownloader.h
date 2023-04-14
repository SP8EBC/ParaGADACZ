/*
 * ForecastDownloader.h
 *
 *  Created on: Jan 16, 2023
 *      Author: mateusz
 */

#ifndef FORECASTDOWNLOADER_H_
#define FORECASTDOWNLOADER_H_

#include "ConfigurationFile.h"
#include "ForecastApi.h"

#include <tuple>
#include <map>

/**
 * Class representing element of cache index
 */
struct ForecastDownloader_CacheIndexElem {
	long timestamp;	//<! Timestamp when this forecast point has ben downloaded from meteoblue
	std::string locationName;	//!< Name of this location
	std::string filename;		//!< Filename where this forecast data is saved on disk
};

class ForecastDownloader {

	const std::shared_ptr<ConfigurationFile> & configurationFile;

	std::shared_ptr<org::openapitools::client::api::ApiConfiguration> apiConfiguration;

	std::shared_ptr<org::openapitools::client::api::ApiClient> apiClient;

	std::shared_ptr<org::openapitools::client::api::ForecastApi> forecastApi;

	std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> allResults;

	bool anyError = false;

	bool anyGood = false;

	std::map<std::string, ForecastDownloader_CacheIndexElem> cacheIndex;

	/**
	 * Saves this weather forecast into JSON file on disk and upates cache
	 */
	bool saveInCache(std::string & data, std::string & name);

	std::shared_ptr<org::openapitools::client::model::Inline_response_200> loadFromCache (std::string name);

protected:
	/**
	 * Loads cache index from disk
	 */
	bool loadCacheIndex();

	std::map<std::string, ForecastDownloader_CacheIndexElem> getCacheIndex() const {
		return cacheIndex;
	}

public:
	ForecastDownloader(std::shared_ptr<ConfigurationFile> & config);
	virtual ~ForecastDownloader();

	/**
	 * Synchronously download all configured meteoblue forecast
	 */
	bool downloadAllMeteoblue();

	/**
	 * Only for presentation purposes!!!
	 *
	 */
	bool createDemoStub();

	/**
	 * Get meteoblue forecast for point name
	 */
	std::shared_ptr<org::openapitools::client::model::Inline_response_200> getForName(std::string forecastPointName);

	/**
	 *
	 */
	std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> & getAllForecast();

	bool isAnyError() const {
		return anyError;
	}

	bool isAnyGood() const {
		return anyGood;
	}
};

#endif /* FORECASTDOWNLOADER_H_ */

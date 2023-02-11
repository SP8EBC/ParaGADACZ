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


class ForecastDownloader {

	const std::shared_ptr<ConfigurationFile> & configurationFile;

	std::shared_ptr<org::openapitools::client::api::ApiConfiguration> apiConfiguration;

	std::shared_ptr<org::openapitools::client::api::ApiClient> apiClient;

	std::shared_ptr<org::openapitools::client::api::ForecastApi> forecastApi;

	std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> allResults;

	bool anyError = false;

public:
	ForecastDownloader(std::shared_ptr<ConfigurationFile> & config);
	virtual ~ForecastDownloader();
	ForecastDownloader(const ForecastDownloader &other);
	ForecastDownloader(ForecastDownloader &&other);
	ForecastDownloader& operator=(const ForecastDownloader &other);
	ForecastDownloader& operator=(ForecastDownloader &&other);

	/**
	 * Synchronously download all configured meteoblue forecast
	 */
	bool downloadAllMeteoblue();

	/**
	 * Get meteoblue forecast for point name
	 */
	std::shared_ptr<org::openapitools::client::model::Inline_response_200> getForName(std::string forecastPointName);

	/**
	 *
	 */
	std::vector<std::tuple<std::string, org::openapitools::client::model::Inline_response_200>> & getAllForecast();
};

#endif /* FORECASTDOWNLOADER_H_ */

/*
 * WeatherlinkDownloader.cpp
 *
 *  Created on: Mar 19, 2023
 *      Author: mateusz
 */

#include "WeatherlinkDownloader.h"

WeatherlinkDownloader::WeatherlinkDownloader(std::shared_ptr<ConfigurationFile> & config)  : configurationFile(config) {

	const utility::string_t pogoda_base_url = "https://api.weatherlink.com/v1/";

	apiConfiguration = std::make_shared<org::openapitools::client::api::ApiConfiguration>();
	apiConfiguration->setBaseUrl(pogoda_base_url);

	apiClient = std::make_shared<org::openapitools::client::api::ApiClient>();
	apiClient->setConfiguration(apiConfiguration);

	conditionsApi = std::make_shared<org::openapitools::client::api::ConditionsApi>(apiClient);

}

WeatherlinkDownloader::~WeatherlinkDownloader() {
}

void WeatherlinkDownloader::downloadForStation(std::string deviceId,
		std::string password, std::string token) {

	downloadedContent = conditionsApi->noaaExtJsonGet(deviceId, password, token).get();

}

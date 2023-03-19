/*
 * WeatherlinkDownloader.h
 *
 *  Created on: Mar 19, 2023
 *      Author: mateusz
 */

#ifndef WEATHERLINKDOWNLOADER_H_
#define WEATHERLINKDOWNLOADER_H_

#include "ConfigurationFile.h"

#include "ConditionsApi.h"

#include <memory>
#include <string>

class WeatherlinkDownloader {

	const std::shared_ptr<ConfigurationFile> & configurationFile;

	std::shared_ptr<org::openapitools::client::api::ApiConfiguration> apiConfiguration;

	std::shared_ptr<org::openapitools::client::api::ApiClient> apiClient;

	/**
	 * api response for previous successfull query
	 */
	std::shared_ptr<org::openapitools::client::model::Root> downloadedContent;

	/**
	 * API endpoint implementation which returns current conditions for requested station
	 */
	std::shared_ptr<org::openapitools::client::api::ConditionsApi> conditionsApi;

public:
	WeatherlinkDownloader(std::shared_ptr<ConfigurationFile> & config);
	virtual ~WeatherlinkDownloader();

	/**
	 * Download raw
	 */
	void downloadForStation(std::string deviceId, std::string password, std::string token);

	const std::shared_ptr<org::openapitools::client::model::Root>& getDownloadedContent() const {
		return downloadedContent;
	}
};

#endif /* WEATHERLINKDOWNLOADER_H_ */

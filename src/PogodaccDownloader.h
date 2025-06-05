/*
 * PogodaccDownloader.h
 *
 *  Created on: Mar 19, 2023
 *      Author: mateusz
 */

#ifndef POGODACCDOWNLOADER_H_
#define POGODACCDOWNLOADER_H_

#include "ConfigurationFile.h"

// api
#include "ApiConfiguration.h"
#include "ApiClient.h"
#include "ListOfAllStationsApi.h"
#include "StationApi.h"

#include <memory>
#include <optional>
#include <functional>

class PogodaccDownloader {

	const std::shared_ptr<ConfigurationFile> & configurationFile;

	std::shared_ptr<org::openapitools::client::api::ApiConfiguration> apiConfiguration;

	std::shared_ptr<org::openapitools::client::api::ApiClient> apiClient;

	//!< Gets a list of all station
	std::shared_ptr<org::openapitools::client::api::ListOfAllStationsApi> listofAllStationApi;

	//!< Gets summary for given station and few more things
	std::shared_ptr<org::openapitools::client::api::StationApi> stationApi;

	//!< List of all station from pogoda.cc meteo_backend
	std::vector<std::shared_ptr<org::openapitools::client::model::StationDefinitionModel>> listOfAllStationsPogodacc;

	bool apiHasFailed;

public:
	PogodaccDownloader(std::shared_ptr<ConfigurationFile> & config);
	virtual ~PogodaccDownloader();

	bool downloadAllStationsList();

	const std::vector<
					std::shared_ptr<
							org::openapitools::client::model::StationDefinitionModel> >&  getListOfAllStationsPogodacc() const {

			return listOfAllStationsPogodacc;
	}

	std::optional<std::reference_wrapper<std::shared_ptr<org::openapitools::client::api::StationApi>>> getStationApi() {
		if (apiHasFailed)
		{
			return {};
		}
		else
		{
			//return std::make_optional<std::reference_wrapper<std::shared_ptr<org::openapitools::client::api::StationApi>>>(stationApi);
			return stationApi;
		}
	}
};

#endif /* POGODACCDOWNLOADER_H_ */

/*
 * PogodaccDownloader.cpp
 *
 *  Created on: Mar 19, 2023
 *      Author: mateusz
 */

#include "PogodaccDownloader.h"

PogodaccDownloader::PogodaccDownloader(std::shared_ptr<ConfigurationFile> & config) : configurationFile(config) {

	const utility::string_t pogoda_base_url = "http://pogoda.cc:8080/meteo_backend_web/";

	apiConfiguration = std::make_shared<org::openapitools::client::api::ApiConfiguration>();
	apiConfiguration->setBaseUrl(pogoda_base_url);

	apiClient = std::make_shared<org::openapitools::client::api::ApiClient>();
	apiClient->setConfiguration(apiConfiguration);

	listofAllStationApi = std::make_shared<org::openapitools::client::api::ListOfAllStationsApi>(apiClient);
	stationApi = std::make_shared<org::openapitools::client::api::StationApi>(apiClient);
}

PogodaccDownloader::~PogodaccDownloader() {
	// TODO Auto-generated destructor stub
}

void PogodaccDownloader::downloadAllStationsList() {
	listOfAllStationsPogodacc = listofAllStationApi->listOfAllStationsGet().get();

}

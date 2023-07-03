/*
 * PogodaccDownloader.cpp
 *
 *  Created on: Mar 19, 2023
 *      Author: mateusz
 */

#include "PogodaccDownloader.h"

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

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
	try {
		listOfAllStationsPogodacc = listofAllStationApi->listOfAllStationsGet().get();
	}
	catch (web::json::json_exception & ex) {
		SPDLOG_ERROR(ex.what());
	}
}

/*
 * CurentConditionsDownloader.h
 *
 *  Created on: Feb 15, 2023
 *      Author: mateusz
 */

#ifndef CURENTCONDITIONSDOWNLOADER_H_
#define CURENTCONDITIONSDOWNLOADER_H_

#include <vector>
#include <memory>
#include <optional>

#include "AprxLogParser.h"
#include "ConfigurationFile.h"

#include "StationApi.h"
#include "ListOfAllStationsApi.h"


class CurentConditionsDownloader {

	CurentConditionsDownloader();
	virtual ~CurentConditionsDownloader();

public:

	static int downloadParseCurrentCondotions(
			std::shared_ptr<std::vector<ConfigurationFile_CurrentWeather>> currentWeatherConfig,
			std::vector<AprsWXData> & currentWeatherAprx,
			std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> & currentWeatherMeteobackend,
			std::vector<std::shared_ptr<org::openapitools::client::model::StationDefinitionModel>> listOfAllStationsPogodacc,
			std::shared_ptr<org::openapitools::client::api::StationApi> stationApi,
			std::optional<float> regionalPressure,
			AprxLogParser & logParser);

};

#endif /* CURENTCONDITIONSDOWNLOADER_H_ */

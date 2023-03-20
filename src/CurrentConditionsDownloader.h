/*
 * CurentConditionsDownloader.h
 *
 *  Created on: Feb 15, 2023
 *      Author: mateusz
 */

#ifndef CURRENTCONDITIONSDOWNLOADER_H_
#define CURRENTCONDITIONSDOWNLOADER_H_

#include <vector>
#include <memory>
#include <optional>

#include "AprxLogParser.h"
#include "ConfigurationFile.h"
#include "PlaylistAssembler.h"
#include "WeatherlinkDownloader.h"

#include "StationApi.h"
#include "ListOfAllStationsApi.h"


class CurrentConditionsDownloader {

	CurrentConditionsDownloader();
	virtual ~CurrentConditionsDownloader();

public:

	static int downloadParseCurrentCondotions(
			std::shared_ptr<std::vector<ConfigurationFile_CurrentWeather>> currentWeatherConfig,
			std::vector<AprsWXData> & currentWeatherAprx,
			std::vector<std::tuple<std::string, AprsWXData>> & currentWeatherDavisWeatherlink,
			std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> & currentWeatherMeteobackend,
			std::vector<std::shared_ptr<org::openapitools::client::model::StationDefinitionModel>> listOfAllStationsPogodacc,
			std::shared_ptr<org::openapitools::client::api::StationApi> stationApi,
			std::optional<float> regionalPressure,
			AprxLogParser & logParser,
			std::shared_ptr<WeatherlinkDownloader> weatherlink);

	static int downloadParseAvalancheWarnings(
			const ConfigurationFile_Avalanche & config,
			AvalancheWarnings & warnings,
			std::shared_ptr<PlaylistAssembler> playlistAssembler
			);

};

#endif /* CURRENTCONDITIONSDOWNLOADER_H_ */

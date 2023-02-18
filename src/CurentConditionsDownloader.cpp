/*
 * CurentConditionsDownloader.cpp
 *
 *  Created on: Feb 15, 2023
 *      Author: mateusz
 */

#include "CurentConditionsDownloader.h"

#include <algorithm>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

CurentConditionsDownloader::CurentConditionsDownloader() {}

CurentConditionsDownloader::~CurentConditionsDownloader() {}

int CurentConditionsDownloader::downloadParseCurrentCondotions(
		std::shared_ptr<std::vector<ConfigurationFile_CurrentWeather> > currentWeatherConfig,
		std::vector<AprsWXData> &currentWeatherAprx,
		std::vector<
				std::pair<std::string,
						std::shared_ptr<
								org::openapitools::client::model::Summary> > > &currentWeatherMeteobackend,
		std::vector<
				std::shared_ptr<
						org::openapitools::client::model::StationDefinitionModel> > listOfAllStationsPogodacc,
		std::shared_ptr<org::openapitools::client::api::StationApi> stationApi,
		std::optional<float> regionalPressure, AprxLogParser &logParser)
{

	// go through configuration and download current weather conditions
	for (ConfigurationFile_CurrentWeather current : *currentWeatherConfig) {
		switch (current.type) {
		case APRX: {
			// split callsign from SSID
			std::tuple<std::string, unsigned> split = ConfigurationFile::splitCallsign(current.name);

			// get last packet for that station
			std::optional<AprsWXData> packet = logParser.getLastPacketForStation(std::get<std::string>(split), std::get<unsigned>(split));

			if (packet.has_value()) {
				// set what should be used from this packet
				packet->useWind = current.sayWind;
				packet->useTemperature = current.sayTemperature;
				packet->useHumidity = current.sayHumidy;
				packet->usePressure = current.sayPressure;

				// check if this station should provide regional pressure value
				if (current.regionalPressure) {

					// check if regional pressure was get before
					if (regionalPressure.has_value()) {
						SPDLOG_ERROR("More than one source of regional pressure found!");

						return -2;
					}
					else {
						regionalPressure = packet->pressure;
					}
				}

				SPDLOG_INFO("Adding current weather conditions from APRX for station: {}", current.name);

				// add this to the vector of current weather
				currentWeatherAprx.push_back(std::move(*packet));
			}
			else {
				SPDLOG_ERROR("Cannot find any current conditions for {} within APRX rf log file!!", current.name);
#ifdef MAIN_FAIL_ON_MISSING_CURRENT_CONDITIONS
				return -3;
#else
#endif
			}

			break;
		}

		case POGODA_CC: {
			// look for weather station in results from meteo_backend
			auto forecast = std::find_if(listOfAllStationsPogodacc.begin(), listOfAllStationsPogodacc.end(), [& current](auto x) {
				if (boost::algorithm::to_upper_copy(x->getName()) == boost::algorithm::to_upper_copy(current.name)) {
					return true;
				}
				else {
					return false;
				}
			});

			// check if this station was found or not
			if (forecast != listOfAllStationsPogodacc.end()) {
				SPDLOG_INFO("Downloading meteo_backend summary for {}", (*forecast)->getName());

				// get summary for that station
				std::shared_ptr<org::openapitools::client::model::Summary> summary = stationApi->stationStationNameSummaryGet((*forecast)->getName()).get();

				SPDLOG_INFO("Adding current weather conditions from meteo_backend for station: {}", current.name);
				SPDLOG_DEBUG("last_timestamp: {}, temperature: {}, wind_speed: {}", summary->getLastTimestamp(), summary->getAvgTemperature(), summary->getAverageSpeed());

				// add this result to
				currentWeatherMeteobackend.push_back({current.name, summary});

			}
			else {
				SPDLOG_ERROR("Cannot find definition for {} within list of all stations from pogoda.cc meteo_backend!!", current.name);
#ifdef MAIN_FAIL_ON_MISSING_CURRENT_CONDITIONS
				return -4;
#else
#endif
			}
			break;
		}
		default:
			SPDLOG_ERROR("Unsupported current weather source");
		}
	}

	return 0;
}

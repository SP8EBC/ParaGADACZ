/*
 * CurentConditionsDownloader.cpp
 *
 *  Created on: Feb 15, 2023
 *      Author: mateusz
 */

#include "CurrentConditionsDownloader.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")


CurrentConditionsDownloader::CurrentConditionsDownloader() {}

CurrentConditionsDownloader::~CurrentConditionsDownloader() {}

int CurrentConditionsDownloader::downloadParseCurrentCondotions(
		std::shared_ptr<std::vector<ConfigurationFile_CurrentWeather> > currentWeatherConfig, 	/* input */
		std::vector<AprsWXData> &currentWeatherAprx,											/* output */
		std::vector<std::tuple<std::string, AprsWXData>> & currentWeatherDavisWeatherlink, 		/* output */
		std::vector<
					std::pair<std::string,
							std::shared_ptr<
									org::openapitools::client::model::Summary> > > &currentWeatherMeteobackend,	/* output */
		const std::vector<
					std::shared_ptr<
							org::openapitools::client::model::StationDefinitionModel> > &listOfAllStationsPogodacc,	/* input */
		std::optional<std::reference_wrapper<std::shared_ptr<org::openapitools::client::api::StationApi>>> stationApi,					/* input */
		std::optional<float> regionalPressure, AprxLogParser &logParser,						/* input */
		std::shared_ptr<WeatherlinkDownloader> weatherlink)										/* input */
{
	bool pogodaCcInop = false;

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

				SPDLOG_INFO("Adding current weather from APRX for station: {}, windpseed: {}, temperature: {}", current.name, packet->wind_speed, packet->temperature);

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

		case DAVIS: {

			// get a device id to download
			std::string did = current.name;

			// download data for that station
			weatherlink->downloadForStation(did);

			// pick up the result
			const std::shared_ptr<org::openapitools::client::model::Root>& weatherlinkResult = weatherlink->getDownloadedContent();

			// if result has been obtained successfully
			if (weatherlinkResult) {
				// convert it to uniersal format
				std::tuple<std::string, AprsWXData> wxdata = WeatherlinkDownloader::convertModelToWxData(weatherlinkResult);

				currentWeatherDavisWeatherlink.push_back(std::move(wxdata));
			}

			break;
		}

		case POGODA_CC: {
			if (pogodaCcInop) {
				break;
			}

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
				try {
					std::shared_ptr<org::openapitools::client::api::StationApi> &api = stationApi.value();

					SPDLOG_INFO("Downloading meteo_backend summary for {}", (*forecast)->getName());

					// get summary for that station
					std::shared_ptr<org::openapitools::client::model::Summary> summary = api->stationStationNameSummaryGet((*forecast)->getName()).get();

					SPDLOG_INFO("Adding current weather from meteo_backend for station: {}", current.name);
					SPDLOG_DEBUG("last_timestamp: {}, temperature: {}, wind_speed: {}", summary->getLastTimestamp(), summary->getAvgTemperature(), summary->getAverageSpeed());

					// add this result to
					currentWeatherMeteobackend.push_back({current.name, summary});
				}
				catch (std::bad_optional_access & ex) {
					SPDLOG_ERROR("meteo_backend api is not available. all further attempts will be skipped");
					pogodaCcInop = true;
				}
				catch (org::openapitools::client::api::ApiException & ex) {
					SPDLOG_ERROR(ex.what());

					pogodaCcInop = true;
				}
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

int CurrentConditionsDownloader::downloadParseAvalancheWarnings(
		const ConfigurationFile_Avalanche &config,
		AvalancheWarnings & warnings,
		std::shared_ptr<PlaylistAssembler> playlistAssembler) {

	int out = 0;

	// if babia gora is enabled
	if (config.goprBabiaGora) {

		// download data
		const int response = warnings.download(AvalancheWarnings_Location::BABIA_GORA);

		// if response was successfull
		if (response == 200) {

			// scrap downloaded data
			warnings.parseLevel();
			warnings.parseDangerousExposition();

			playlistAssembler->avalancheWarning(
					AvalancheWarnings_Location::BABIA_GORA,
					warnings.getSeverityLevel(),
					warnings.getCurrentExpositions());

		}
		else {
			SPDLOG_ERROR("Avalanche warning data cannot be downloaded from GOPR website!!!");

			out = -1;
		}
	}

	return out;
}

/*
 * TrendDownloader.cpp
 *
 *  Created on: Aug 16, 2023
 *      Author: mateusz
 */

#include "TrendDownloader.h"
#include "TimeTools.h"

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

TrendDownloader::TrendDownloader() {
	// TODO Auto-generated constructor stub

}

TrendDownloader::~TrendDownloader() {
	// TODO Auto-generated destructor stub
}

int TrendDownloader::downloadTrendData(std::vector<TrendDownloader_Data> &out,
		const ConfigurationFile &config,
		std::optional<std::reference_wrapper<std::shared_ptr<org::openapitools::client::api::StationApi>>> stationApi,
		AprxLogParser &logParser,
		int64_t currentEpoch) {

	int ret = 0;

	bool goodData = false;

	bool pogodaCcInop = false;

	// get trend configuration
	ConfigurationFile_Trend trendConfig = config.getTrend();

	// return if no trend announcement are enabled
	if (!trendConfig.enabledTemperature && !trendConfig.enabledWindspeed) {
		SPDLOG_INFO("Trend data is disabled by configuration file");
		return ret;
	}

	// iterate through all configured sources of current conditions
	for (ConfigurationFile_CurrentWeather current : config.getCurrent()) {

		SPDLOG_INFO("Looking for trend archival data for station {}", current.name);

		float temperatureArchivalAverage = 0.0f;
		float windspeedArchivalAverage = 0.0f;

		// different source types require different handling
		switch (current.type) {
		case APRX: {
			if (trendConfig.trendLenghtInHours == 0) {
				SPDLOG_WARN("Trend lenght shall be set to minimum 1 hour");
				goodData = false;
			}
			else {
				AprsWXData average;
				average.useTemperature = true;
				average.useWind = true;

				// get current time
				const long time = currentEpoch;

				// configured trend length is a center time for calculation
				const int trendBegin = time - (trendConfig.trendLenghtInHours * 3600) - 1200;
				const int trendEnd = time - (trendConfig.trendLenghtInHours * 3600) + 1200;

				// get all weather packets between given time
				std::vector<AprsWXData> packets = logParser.getAllWeatherPacketsInTimerange(trendBegin, trendEnd);

				// filter packets
				std::vector<AprsWXData> filteredPackets = AprxLogParser::filterPacketsPerCallsign(current.name, 0, packets);

				if (filteredPackets.size() == 0) {
					SPDLOG_WARN("No archival data in APRX log file between {} and {}", trendBegin, trendEnd);

					goodData = false;
				}
				else {

					SPDLOG_DEBUG("{} packets have been found in APRX rf log file, {} remained after filtering", packets.size(), filteredPackets.size());
					SPDLOG_DEBUG("Oldest packet to be used: {}, Newest packet to be used: {}",
																							boost::posix_time::to_simple_string(filteredPackets.at(0).packetLocalTimestmp),
																							boost::posix_time::to_simple_string(filteredPackets.at(filteredPackets.size() - 1).packetLocalTimestmp));

					// sum values from all packets obtained
					average = std::accumulate(filteredPackets.begin(), filteredPackets.end(), average, [] (AprsWXData & accumulator, AprsWXData & element) {

						// set use flag to make '+' operator working
						element.useTemperature = true;
						element.useWind = true;

						return std::move(accumulator) + element;
					});

					// calculate average itself
					windspeedArchivalAverage = average.wind_speed / filteredPackets.size();
					temperatureArchivalAverage = average.temperature / filteredPackets.size();

					SPDLOG_DEBUG("Average from {} hours ago for station {}, temperature: {}, wind: {}", trendConfig.trendLenghtInHours, current.name, temperatureArchivalAverage, windspeedArchivalAverage);

					goodData = true;
				}
			}

			break;
		}
		case POGODA_CC: {
			if (pogodaCcInop) {
				break;
			}

			try {
				std::shared_ptr<org::openapitools::client::api::StationApi> &api = stationApi.value();

				// get trend data for currently processed station
				std::shared_ptr<org::openapitools::client::model::Trend> trend = api->stationStationNameTrendGet(current.name).get();

				// extract data for temperature and windspeed
				std::shared_ptr<org::openapitools::client::model::TrendData> temperatureTrend = trend->getTemperatureTrend();
				std::shared_ptr<org::openapitools::client::model::TrendData> windspeedTrend = trend->getAverageWindSpeedTrend();

				goodData = true;

				// pogoda.cc backend returns trend for certain time windows.
				switch (trendConfig.trendLenghtInHours) {
				case 2:
					temperatureArchivalAverage = temperatureTrend->getTwoHoursValue();
					windspeedArchivalAverage = windspeedTrend->getTwoHoursValue();
					SPDLOG_DEBUG("Average from two hours ago for station {}, temperature: {}, wind: {}", current.name, temperatureArchivalAverage, windspeedArchivalAverage);
					break;
				case 4:
					temperatureArchivalAverage = temperatureTrend->getFourHoursValue();
					windspeedArchivalAverage = windspeedTrend->getFourHoursValue();
					SPDLOG_DEBUG("Average from four hours ago for station {}, temperature: {}, wind: {}", current.name, temperatureArchivalAverage, windspeedArchivalAverage);
					break;
				case 6:
					temperatureArchivalAverage = temperatureTrend->getSixHoursValue();
					windspeedArchivalAverage = windspeedTrend->getSixHoursValue();
					SPDLOG_DEBUG("Average from six hours ago for station {}, temperature: {}, wind: {}", current.name, temperatureArchivalAverage, windspeedArchivalAverage);
					break;
				case 8:
					temperatureArchivalAverage = temperatureTrend->getEightHoursValue();
					windspeedArchivalAverage = windspeedTrend->getEightHoursValue();
					SPDLOG_DEBUG("Average from eight hours ago for station {}, temperature: {}, wind: {}", current.name, temperatureArchivalAverage, windspeedArchivalAverage);
					break;
				default:
					goodData = false;
					SPDLOG_ERROR("pogoda.cc meteo backend provide trend data only for 2, 4, 6 and 8 hours timeframe");
				}
			}
			catch (std::bad_optional_access & ex) {
				SPDLOG_ERROR("meteo_backend api is not available. all further attempts to download trend data will be skipped");
				pogodaCcInop = true;
			}
			catch (org::openapitools::client::api::ApiException & ex) {
				SPDLOG_ERROR(ex.what());

				pogodaCcInop = true;
			}

			break;
		}
		default:
			SPDLOG_WARN("Current weather source of type {} is not supported yet", current.type);
		}

		if (goodData) {
			ret++;
			out.emplace_back(current.name, current.type, trendConfig.trendLenghtInHours, ::round(windspeedArchivalAverage), ::round(temperatureArchivalAverage));
		}

	}


	return ret;
}

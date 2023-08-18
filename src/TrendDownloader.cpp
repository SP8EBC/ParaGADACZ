/*
 * TrendDownloader.cpp
 *
 *  Created on: Aug 16, 2023
 *      Author: mateusz
 */

#include "TrendDownloader.h"

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
		std::shared_ptr<org::openapitools::client::api::StationApi> stationApi,
		AprxLogParser &logParser) {

	int ret = 0;

	bool goodData = false;

	// get trend configuration
	ConfigurationFile_Trend trendConfig = config.getTrend();

	// return if no trend announcement are enabled
	if (!trendConfig.enabledTemperature && !trendConfig.enabledWindspeed) {
		return ret;
	}

	// iterate through all configured sources of current conditions
	for (ConfigurationFile_CurrentWeather current : config.getCurrent()) {

		float temperatureTrendVal = 0.0f;
		float windspeedTrendVal = 0.0f;

		// different source types require different handling
		switch (current.type) {
		case POGODA_CC: {
			// get trend data for currently processed station
			std::shared_ptr<org::openapitools::client::model::Trend> trend = stationApi->stationStationNameTrendGet(current.name).get();

			// extract data for temperature and windspeed
			std::shared_ptr<org::openapitools::client::model::TrendData> temperatureTrend = trend->getTemperatureTrend();
			std::shared_ptr<org::openapitools::client::model::TrendData> windspeedTrend = trend->getAverageWindSpeedTrend();

			goodData = true;

			// pogoda.cc backend returns trend for certain time windows.
			switch (trendConfig.trendLenghtInHours) {
			case 2:
				temperatureTrendVal = temperatureTrend->getTwoHoursValue();
				windspeedTrendVal = windspeedTrend->getTwoHoursValue();
				SPDLOG_DEBUG("Two hour trend for station {}, temperature: {}, wind: {}", current.name, temperatureTrendVal, windspeedTrendVal);
				break;
			case 4:
				temperatureTrendVal = temperatureTrend->getFourHoursValue();
				windspeedTrendVal = windspeedTrend->getFourHoursValue();
				SPDLOG_DEBUG("Four hour trend for station {}, temperature: {}, wind: {}", current.name, temperatureTrendVal, windspeedTrendVal);
				break;
			case 6:
				temperatureTrendVal = temperatureTrend->getSixHoursValue();
				windspeedTrendVal = windspeedTrend->getSixHoursValue();
				SPDLOG_DEBUG("Six hour trend for station {}, temperature: {}, wind: {}", current.name, temperatureTrendVal, windspeedTrendVal);
				break;
			case 8:
				temperatureTrendVal = temperatureTrend->getEightHoursValue();
				windspeedTrendVal = windspeedTrend->getEightHoursValue();
				SPDLOG_DEBUG("Eight hour trend for station {}, temperature: {}, wind: {}", current.name, temperatureTrendVal, windspeedTrendVal);
				break;
			default:
				goodData = false;
				SPDLOG_ERROR("pogoda.cc meteo backend provide trend data only for 2, 4, 6 and 8 hours timeframe");
			}

			break;
		}
		default:
			SPDLOG_WARN("Current weather source of type {} is not supported yet", current.type);
		}

		if (goodData) {
			ret++;
			out.emplace_back(current.name, current.type, trendConfig.trendLenghtInHours, ::round(windspeedTrendVal), ::round(temperatureTrendVal));
		}

	}


	return ret;
}

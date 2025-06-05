/*
 * TrendDownloader.h
 *
 *  Created on: Aug 16, 2023
 *      Author: mateusz
 */

#ifndef TRENDDOWNLOADER_H_
#define TRENDDOWNLOADER_H_

#include "AprxLogParser.h"
#include "ConfigurationFile.h"
#include "StationApi.h"

#include <vector>

template<typename Time>
class TrendDownloader_GetTime {
public:

	int64_t getTime() {
		return Time::getEpoch();
	}
};

class TrendDownloader_Data {
public:

	TrendDownloader_Data(
			std::string _stationName,
			ConfigurationFile_CurrentWeatherType _stationType,
			int _trendLenght,
			int _windspeedTrend,
			int _temperatureTrend) :
					stationName(_stationName),
					stationType(_stationType),
					trendLenght(_trendLenght),
					windspeedTrend(_windspeedTrend),
					temperatureTrend(_temperatureTrend) { ; }

	std::string stationName;		///!< Station name or other identification per source
	ConfigurationFile_CurrentWeatherType stationType;	///!< Station type
	int trendLenght;
	int windspeedTrend;
	int temperatureTrend;
};

/***
 *
 */
class TrendDownloader {
	TrendDownloader();
	virtual ~TrendDownloader();

public:

	static int downloadTrendData(std::vector<TrendDownloader_Data> & out,
								const ConfigurationFile & config,
								std::optional<std::reference_wrapper<std::shared_ptr<org::openapitools::client::api::StationApi>>> stationApi,
								AprxLogParser & logParser,
								int64_t currentEpoch);
};

#endif /* TRENDDOWNLOADER_H_ */

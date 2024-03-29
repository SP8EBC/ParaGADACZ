/*
 * ForecastFinder.h
 *
 * This class used data downloaded from forecast API (meteoblue) to look for weather forecast
 * for given location and a timepoint
 *
 *  Created on: Dec 18, 2022
 *      Author: mateusz
 */

#ifndef FORECASTFINDER_H_
#define FORECASTFINDER_H_

#include <cstdint>
#include <tuple>
#include "Inline_response_200.h"
#include "ConfigurationFile.h"
#include "StationApi.h"
#include "AprsWXData.h"
#include "MeteobluePictocode.h"

class ForecastFinder {

public:
	static std::tuple<int64_t, float> getTemperatureMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData, uint16_t minutesFromNow);

	static std::tuple<int64_t, float> getWindSpeedMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData, uint16_t minutesFromNow);

	// meteoblue doesn't return windgusts??
	//static std::tuple<int64_t, float> getWindGustsMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData, uint16_t minutesFromNow);

	static std::tuple<int64_t, float> getWindDirectionMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData, uint16_t minutesFromNow);

	static float getRegionalPressure(ConfigurationFile & config, std::vector<org::openapitools::client::model::Summary> & summary, std::vector<AprsWXData> & result);

	static std::tuple<int64_t, utility::string_t> getRainSpotMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData, uint16_t minutesFromNow);

	static std::tuple<int64_t, MeteobluePictocode> getPictocodeMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData, uint16_t minutesFromNow);

private:
	ForecastFinder();
	virtual ~ForecastFinder();


};

#endif /* FORECASTFINDER_H_ */

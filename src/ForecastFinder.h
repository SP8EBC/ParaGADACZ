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

class ForecastFinder {

public:
	static std::tuple<int64_t, float> getTemperatureMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData, uint16_t minutesFromNow);

private:
	ForecastFinder();
	virtual ~ForecastFinder();
	ForecastFinder(const ForecastFinder &other);
	ForecastFinder(ForecastFinder &&other);
	ForecastFinder& operator=(const ForecastFinder &other);
	ForecastFinder& operator=(ForecastFinder &&other);


};

#endif /* FORECASTFINDER_H_ */

/*
 * ForecastFinder.cpp
 *
 * This class used data downloaded from forecast API (meteoblue) to look for weather forecast
 * for given location and a timepoint
 *
 *  Created on: Dec 18, 2022
 *      Author: mateusz
 */

#include "ForecastFinder.h"

#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o


ForecastFinder::ForecastFinder() {

}

ForecastFinder::~ForecastFinder() {
}


std::tuple<int64_t, float> ForecastFinder::getTemperatureMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, float> out;

	// index of forecast timepoint to use
	int index = 0;

	// get currrent timestamp
	boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

	// epoch
	boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

	// timestamp
	long ts = (current - epoch).total_seconds();

	// get data
	std::shared_ptr<org::openapitools::client::model::Data_3h> data = meteoblueData->getData3h();

	// iterate through all timestamps
	for (int64_t tim : data->getTime()) {
		// break on first poinr after
		if (tim > (ts + minutesFromNow * 60)) {
			out = std::make_tuple(tim, data->getTemperature().at(index));

			break;
		}

		index++;
	}

	return out;
}

std::tuple<int64_t, float> ForecastFinder::getWindSpeedMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, float> out;

	// index of forecast timepoint to use
	int index = 0;

	// get currrent timestamp
	boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

	// epoch
	boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

	// timestamp
	long ts = (current - epoch).total_seconds();

	// get data
	std::shared_ptr<org::openapitools::client::model::Data_3h> data = meteoblueData->getData3h();

	// iterate through all timestamps
	for (int64_t tim : data->getTime()) {
		// break on first poinr after
		if (tim > (ts + minutesFromNow * 60)) {
			out = std::make_tuple(tim, data->getWindspeed().at(index));

			break;
		}

		index++;
	}

	return out;


}

std::tuple<int64_t, float> ForecastFinder::getWindDirectionMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, float> out;

	// index of forecast timepoint to use
	int index = 0;

	// get currrent timestamp
	boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

	// epoch
	boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

	// timestamp
	long ts = (current - epoch).total_seconds();

	// get data
	std::shared_ptr<org::openapitools::client::model::Data_3h> data = meteoblueData->getData3h();

	// iterate through all timestamps
	for (int64_t tim : data->getTime()) {
		// break on first poinr after
		if (tim > (ts + minutesFromNow * 60)) {
			out = std::make_tuple(tim, data->getWinddirection().at(index));

			break;
		}

		index++;
	}

	return out;


}

float ForecastFinder::getRegionalPressure(ConfigurationFile &config,
		std::vector<org::openapitools::client::model::Summary> &summary,
		std::vector<AprsWXData> &result) {

	float out = 0.0f;


	return out;
}

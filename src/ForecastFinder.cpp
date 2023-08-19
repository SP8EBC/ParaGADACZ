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
#include "TimeTools.h"
#include "exception/NoMeteoblueForecastForGivenTimeEx.h"

#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o

#include <cmath>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

ForecastFinder::ForecastFinder() {

}

ForecastFinder::~ForecastFinder() {
}


std::tuple<int64_t, float> ForecastFinder::getTemperatureMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, float> out;

	// set to true if forecast value has been found for given time
	bool found = false;

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

			const float foundTemp = data->getTemperature().at(index);

			out = std::make_tuple(tim, foundTemp);

			if (isnan(foundTemp)) {
				SPDLOG_ERROR("Temperature is NaN!, index: {}, tim: {}", index, tim);
			}

			found = true;

			break;
		}

		index++;
	}

	if (!found) {
		throw NoMeteoblueForecastForGivenTimeEx("temperature", minutesFromNow);
	}

	return out;
}

std::tuple<int64_t, float> ForecastFinder::getWindSpeedMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, float> out;

	// set to true if forecast value has been found for given time
	bool found = false;

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

			const float foundWs = data->getWindspeed().at(index);

			out = std::make_tuple(tim, foundWs);

			if (isnan(foundWs)) {
				SPDLOG_ERROR("Wind speed is NaN!, index: {}, tim: {}", index, tim);
			}

			found = true;

			break;
		}

		index++;
	}

	if (!found) {
		throw NoMeteoblueForecastForGivenTimeEx("windspeed", minutesFromNow);
	}

	return out;


}

std::tuple<int64_t, float> ForecastFinder::getWindDirectionMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, float> out;

	// set to true if forecast value has been found for given time
	bool found = false;

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

			found = true;

			break;
		}

		index++;
	}

	if (!found) {
		throw NoMeteoblueForecastForGivenTimeEx("winddirection", minutesFromNow);
	}

	return out;


}

float ForecastFinder::getRegionalPressure(ConfigurationFile &config,
		std::vector<org::openapitools::client::model::Summary> &summary,
		std::vector<AprsWXData> &result) {

	float out = 0.0f;


	return out;
}

std::tuple<int64_t, utility::string_t> ForecastFinder::getRainSpotMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, utility::string_t> out;

	// set to true if forecast value has been found for given time
	bool found = false;

	long current_timestamp = TimeTools::getEpoch();

	// index of forecast timepoint to use
	int index = 0;

	// get data
	std::shared_ptr<org::openapitools::client::model::Data_3h> data = meteoblueData->getData3h();

	// iterate through all timestamps
	for (int64_t tim : data->getTime()) {
		// break on first poinr after
		if (tim > (current_timestamp + minutesFromNow * 60)) {
			out = std::make_tuple(tim, data->getRainspot().at(index));

			SPDLOG_DEBUG("Found rain forecast data, rainspot: {}", data->getRainspot().at(index));

			found = true;

			break;
		}

		index++;
	}

	if (!found) {
		throw NoMeteoblueForecastForGivenTimeEx("rainspot", minutesFromNow);
	}

	return out;

}

std::tuple<int64_t, MeteobluePictocode> ForecastFinder::getPictocodeMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> meteoblueData,
		uint16_t minutesFromNow) {

	std::tuple<int64_t, MeteobluePictocode> out;

	// set to true if forecast value has been found for given time
	bool found = false;

	int32_t found_pictocode;
	int64_t found_time;

	long current_timestamp = TimeTools::getEpoch();

	// index of forecast timepoint to use
	int index = 0;

	// get data
	std::shared_ptr<org::openapitools::client::model::Data_3h> data = meteoblueData->getData3h();

	// iterate through all timestamps
	for (int64_t tim : data->getTime()) {
		// break on first poinr after
		if (tim > (current_timestamp + minutesFromNow * 60)) {
			found_pictocode = data->getPictocode().at(index);
			found_time = tim;

			SPDLOG_DEBUG("Found rain forecast data, pictocode: {}, time: {}", found_pictocode, found_time);

			found = true;

			break;
		}

		index++;
	}

	if (!found) {
		throw NoMeteoblueForecastForGivenTimeEx("pictocode", minutesFromNow);
	}


	out = std::make_tuple(found_time, MeteobluePictocode_valueOf(found_pictocode));

	return out;
}

/*
 * AprsWXDataFactory.cpp
 *
 *  Created on: Feb 11, 2023
 *      Author: mateusz
 */

#include "AprsWXDataFactory.h"

AprsWXDataFactory::AprsWXDataFactory() {

}

AprsWXDataFactory::~AprsWXDataFactory() {
}


AprsWXData AprsWXDataFactory::FromSummaryApiModel(
		org::openapitools::client::model::Summary &model) {

	AprsWXData out;

	out.wind_speed = model.getAverageSpeed();
	out.wind_gusts = model.getGusts();
	out.wind_direction = model.getDirection();
	out.temperature = model.getAvgTemperature();
	out.humidity = model.getHumidity();
	out.pressure = model.getQnh();

	out.useHumidity = true;
	out.usePressure = true;
	out.useTemperature = true;
	out.useWind = true;

	return out;
}

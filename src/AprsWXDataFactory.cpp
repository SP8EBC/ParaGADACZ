/*
 * AprsWXDataFactory.cpp
 *
 *  Created on: Feb 11, 2023
 *      Author: mateusz
 */

#include "AprsWXDataFactory.h"

AprsWXDataFactory::AprsWXDataFactory() {
	// TODO Auto-generated constructor stub

}

AprsWXDataFactory::~AprsWXDataFactory() {
	// TODO Auto-generated destructor stub
}

AprsWXDataFactory::AprsWXDataFactory(const AprsWXDataFactory &other) {
	// TODO Auto-generated constructor stub

}

AprsWXDataFactory::AprsWXDataFactory(AprsWXDataFactory &&other) {
	// TODO Auto-generated constructor stub

}

AprsWXDataFactory& AprsWXDataFactory::operator=(
		const AprsWXDataFactory &other) {
	// TODO Auto-generated method stub

}

AprsWXDataFactory& AprsWXDataFactory::operator=(AprsWXDataFactory &&other) {
	// TODO Auto-generated method stub

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

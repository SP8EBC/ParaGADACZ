/*
 * WeatherlinkDownloader.cpp
 *
 *  Created on: Mar 19, 2023
 *      Author: mateusz
 */

#include "WeatherlinkDownloader.h"

#include <boost/lexical_cast.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

WeatherlinkDownloader::WeatherlinkDownloader(std::shared_ptr<ConfigurationFile> & config)  : configurationFile(config) {

	const utility::string_t pogoda_base_url = "https://api.weatherlink.com/v1/";

	apiConfiguration = std::make_shared<org::openapitools::client::api::ApiConfiguration>();
	apiConfiguration->setBaseUrl(pogoda_base_url);

	apiClient = std::make_shared<org::openapitools::client::api::ApiClient>();
	apiClient->setConfiguration(apiConfiguration);

	conditionsApi = std::make_shared<org::openapitools::client::api::ConditionsApi>(apiClient);

}

WeatherlinkDownloader::~WeatherlinkDownloader() {
}

void WeatherlinkDownloader::downloadForStation(std::string deviceId) {

	try {
		downloadedContent = conditionsApi->noaaExtJsonGet(
								deviceId,
								configurationFile->getSecrets().weatherlinkPassword,
								configurationFile->getSecrets().weatherlinkToken).get();

		SPDLOG_INFO("Weatherlink data station with devide id {} downloaded, station name is: {}", deviceId, downloadedContent->getStationId());
		SPDLOG_INFO("Weatherlink temperature: {}, wind speed in mph!!: {}, windgust in mph!!: {}", downloadedContent->getTempC(), downloadedContent->getDavisCurrentObservation()->getWindTenMinAvgMph(), downloadedContent->getDavisCurrentObservation()->getWindTenMinGustMph());
	}
	catch (org::openapitools::client::api::ApiException & e) {
		SPDLOG_ERROR("ApiException has happened during downloading Weatherlink data for DID {}", deviceId );

		downloadedContent.reset();
	}

}

std::tuple<std::string, AprsWXData> WeatherlinkDownloader::convertModelToWxData(
		std::shared_ptr<org::openapitools::client::model::Root> input) {

	AprsWXData out;

	std::string temp;

	const int observationAge = input->getDavisCurrentObservation()->getObservationAge();
	out.packetAgeInSecondsLocal = observationAge;

	// interpret station-id as callsign
	out.call = input->getStationId();
	out.ssid = 0;
	out.is_primary = true;
	out.is_secondary = false;

	// get wind speed in mph as string
	temp = input->getDavisCurrentObservation()->getWindTenMinAvgMph();

	// convert string to float (in miles per hour)
	out.wind_speed = boost::lexical_cast<float, std::string>(temp);

	// convert wind speed from miles per hour to m/s
	out.wind_speed *= 0.44f;

	// the same but wind gusts
	temp = input->getDavisCurrentObservation()->getWindTenMinGustMph();
	out.wind_gusts = boost::lexical_cast<float, std::string>(temp);
	out.wind_gusts *= 0.44f;

	// wind direction
	temp = input->getWindDegrees();
	out.wind_direction = boost::lexical_cast<int, std::string>(temp);

	// temprature
	temp = input->getTempC();
	out.temperature = boost::lexical_cast<float, std::string>(temp);

	// pressure
	temp = input->getPressureMb();
	out.pressure = (short)boost::lexical_cast<float, std::string>(temp);

	// humidity
	temp = input->getRelativeHumidity();
	out.humidity = boost::lexical_cast<short, std::string>(temp);

	/**
		../src/WeatherlinkDownloader.cpp:79:104: error: cannot bind rvalue reference of type ‘AprsWXData&&’ to lvalue of type ‘AprsWXData’
	 *
	 *
	 *	return std::make_tuple<std::string, AprsWXData>(input->getDavisCurrentObservation()->getDID(), std::forward<AprsWXData>(out));
	 *
	 *
	 * https://stackoverflow.com/questions/69932577/why-does-stdmake-tuple-require-move-semantics
	 */

	return std::make_tuple(input->getDavisCurrentObservation()->getDID(), out);
}

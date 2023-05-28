/*
 * PlaylistAssembler_test.cpp
 *
 *  Created on: Jan 11, 2023
 *      Author: mateusz
 */

#define BOOST_TEST_MODULE PlaylistAssembler_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include <memory>
#include <utility>

#include "PlaylistSamplerPL.h"
#include "PlaylistAssembler.h"
#include "ConfigurationFile.h"
#include "AvalancheWarnings.h"

#include "TimeTools.h"
#include "QualityFactor.h"

#include "PlaylistSamplerPL_files.h"

std::shared_ptr<ConfigurationFile> configuration_file_first; //("./test_input/configuration_playlist_assembler_first.conf");
std::shared_ptr<ConfigurationFile> configuration_file_second; //("./test_input/configuration_playlist_assembler_second.conf");
std::shared_ptr<PlaylistSampler> playlist_sampler;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/playlistassembler_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

    configuration_file_first = std::make_shared<ConfigurationFile>("./test_input/configuration_playlist_assembler_first.conf");
    configuration_file_second = std::make_shared<ConfigurationFile>("./test_input/configuration_playlist_assembler_second.conf");

    configuration_file_first->parse();
    configuration_file_second->parse();
    playlist_sampler = std::make_shared<PlaylistSamplerPL>(configuration_file_first); 	// used for 'getAudioForStationName' and
    																					// 'getAudioForForecast' name
  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

//void PlaylistAssembler::currentWeather(
//		std::vector<std::pair<std::string, org::openapitools::client::model::Summary>> & summary,
//		std::vector<AprsWXData> & result) {

BOOST_AUTO_TEST_CASE(meteoblue) {
	PlaylistAssembler assembler(playlist_sampler, configuration_file_second);
	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(configuration_file_second);

	// std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> & forecasts

	// get currrent timestamp
	boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

	// epoch
	boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

	// timestamp
	long ts = (current - epoch).total_seconds();

	// forecast future time in minutes!!!
	uint32_t future_time = configuration_file_second->getForecast().futureTime;

	//org::openapitools::client::model::Inline_response_200
	auto meteoblueResponse = std::make_shared<org::openapitools::client::model::Inline_response_200>();
	auto data3h = std::make_shared<org::openapitools::client::model::Data_3h>();

	std::vector<int32_t> time;
	std::vector<float> temperature;
	std::vector<int32_t> winddirection;
	std::vector<float> windspeed;

	float expected_temperature = 0.0f;
	int32_t expected_winddrection = 0;
	float expected_windspeed = 0.0f;

	int64_t expected_timestamp = ts + (future_time / 30) * 1800;
	BOOST_TEST_MESSAGE("expected_timestamp: ");
	BOOST_TEST_MESSAGE(expected_timestamp);

	////////////// prepare stubbed weather forecast data
	for (int i = 0; i < 64; i++) {
		time.push_back(ts + i * 1800);
		temperature.push_back(20.0f + 0.1f * i);
		windspeed.push_back(1.0f + 0.1f * i);
		winddirection.push_back(180 + i);

		if (expected_timestamp < (ts + (i + 1) * 1800)) {
			if (expected_timestamp >= (ts + i * 1800)) {
				///////////////////////////////////////////
				expected_temperature = 20.0f + 0.1f * (i + 1);
				expected_winddrection = 180 + i + 1;
				expected_windspeed = 1.0f + 0.1f * (i + 1);
				///////////////////////////////////////////
				BOOST_TEST_MESSAGE("expected_temperature: ");
				BOOST_TEST_MESSAGE(expected_temperature);
				BOOST_TEST_MESSAGE("expected_windspeed: ");
				BOOST_TEST_MESSAGE(expected_windspeed);
				BOOST_TEST_MESSAGE("expected_winddirection: ");
				BOOST_TEST_MESSAGE(expected_winddrection);
			}
		}
	}


	std::vector<std::string> playlist_expected_temperature = c->getAudioListFromNumber(expected_temperature);
	std::vector<std::string> playlist_expected_windspeed = c->getAudioListFromNumber(expected_windspeed);
	std::vector<std::string> playlist_expected_winddir = c->getAudioListFromNumber(expected_winddrection);


	data3h->setTime(time);
	data3h->setTemperature(temperature);
	data3h->setWinddirection(winddirection);
	data3h->setWindspeed(windspeed);

	meteoblueResponse->setData3h(data3h);

	std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>> skrzyczne = std::make_tuple("skrzyczne", meteoblueResponse);
	std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>> jezioro = std::make_tuple("jezioro", meteoblueResponse);


	std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> vct;
	vct.push_back(skrzyczne);
	vct.push_back(jezioro);

	//////////////////////////// assemble playlist
	assembler.start();
	BOOST_CHECK_NO_THROW(assembler.forecastMeteoblue(vct));
	////////////////////////////

	std::shared_ptr<std::vector<std::string>> playlist_ptr = assembler.getPlaylist();
	std::vector<std::string> playlist = *playlist_ptr;

	int i = 0;

	//////////////////////////////////// checks for playlist content starts here
	BOOST_CHECK_EQUAL("ident.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(PROGNOZA, playlist[i++]);
	BOOST_CHECK_EQUAL(NA_NASTEPNE, playlist[i++]);
	switch (future_time / 60) {
	case 1: BOOST_CHECK_EQUAL(NUMBER_1, playlist[i++]); 	BOOST_CHECK_EQUAL(HOUR_ONE, playlist[i++]);break;
	case 2:	BOOST_CHECK_EQUAL(NUMBER_2_, playlist[i++]);	BOOST_CHECK_EQUAL(HOUR_TWO_FOUR, playlist[i++]);break;
	case 3:	BOOST_CHECK_EQUAL(NUMBER_3, playlist[i++]); 	BOOST_CHECK_EQUAL(HOUR_TWO_FOUR, playlist[i++]);break;
	case 4:	BOOST_CHECK_EQUAL(NUMBER_4, playlist[i++]); 	BOOST_CHECK_EQUAL(HOUR_TWO_FOUR, playlist[i++]);break;
	case 5:	BOOST_CHECK_EQUAL(NUMBER_5, playlist[i++]); 	BOOST_CHECK_EQUAL(HOUR_FOUR, playlist[i++]);break;
	case 6:	BOOST_CHECK_EQUAL(NUMBER_6, playlist[i++]); 	BOOST_CHECK_EQUAL(HOUR_FOUR, playlist[i++]);break;
	case 7:	BOOST_CHECK_EQUAL(NUMBER_7, playlist[i++]);	 	BOOST_CHECK_EQUAL(HOUR_FOUR, playlist[i++]);break;
	case 8:	BOOST_CHECK_EQUAL(NUMBER_8, playlist[i++]); 	BOOST_CHECK_EQUAL(HOUR_FOUR, playlist[i++]);break;
	case 9:	BOOST_CHECK_EQUAL(NUMBER_9, playlist[i++]); 	BOOST_CHECK_EQUAL(HOUR_FOUR, playlist[i++]);break;
	default: BOOST_CHECK(false);
	}
	BOOST_CHECK_EQUAL(configuration_file_second->getForecast().locations[0].nameIdent, playlist[i++]);
	BOOST_TEST_MESSAGE(configuration_file_second->getForecast().locations[0].nameIdent);
	if (configuration_file_second->getForecast().locations[0].sayWind) {
		BOOST_CHECK_EQUAL(KIERUNEK_WIATRU, playlist[i++]);
		if ((expected_winddrection <= 11 && expected_winddrection >= 0) || (expected_winddrection > 349 && expected_winddrection < 360))
			BOOST_CHECK_EQUAL(DIRECTION_N, playlist[i++]);
		else if (expected_winddrection <= 34 && expected_winddrection > 11)
			BOOST_CHECK_EQUAL(DIRECTION_NNE, playlist[i++]);
		else if (expected_winddrection <= 56 && expected_winddrection > 34)
			BOOST_CHECK_EQUAL(DIRECTION_NE, playlist[i++]);
		else if (expected_winddrection <= 79 && expected_winddrection > 56)
			BOOST_CHECK_EQUAL(DIRECTION_ENE, playlist[i++]);
		else if (expected_winddrection <= 101 && expected_winddrection > 79)
			BOOST_CHECK_EQUAL(DIRECTION_E, playlist[i++]);
		else if (expected_winddrection <= 124 && expected_winddrection > 101)
			BOOST_CHECK_EQUAL(DIRECTION_ESE, playlist[i++]);
		else if (expected_winddrection <= 146 && expected_winddrection > 124)
			BOOST_CHECK_EQUAL(DIRECTION_SE, playlist[i++]);
		else if (expected_winddrection <= 169 && expected_winddrection > 146)
			BOOST_CHECK_EQUAL(DIRECTION_SSE, playlist[i++]);
		else if (expected_winddrection <= 191 && expected_winddrection > 169)
			BOOST_CHECK_EQUAL(DIRECTION_S, playlist[i++]);
		else if (expected_winddrection <= 214 && expected_winddrection > 191)
			BOOST_CHECK_EQUAL(DIRECTION_SSW, playlist[i++]);
		else if (expected_winddrection <= 236 && expected_winddrection > 214)
			BOOST_CHECK_EQUAL(DIRECTION_SW, playlist[i++]);
		else if (expected_winddrection <= 259 && expected_winddrection > 236)
			BOOST_CHECK_EQUAL(DIRECTION_WSW, playlist[i++]);
		else if (expected_winddrection <= 281 && expected_winddrection > 259)
			BOOST_CHECK_EQUAL(DIRECTION_W, playlist[i++]);
		else if (expected_winddrection <= 304 && expected_winddrection > 281)
			BOOST_CHECK_EQUAL(DIRECTION_WNW, playlist[i++]);
		else if (expected_winddrection <= 327 && expected_winddrection > 304)
			BOOST_CHECK_EQUAL(DIRECTION_NW, playlist[i++]);
		else if (expected_winddrection <= 349 && expected_winddrection > 327)
			BOOST_CHECK_EQUAL(DIRECTION_NNW, playlist[i++]);
		for (std::string elem : playlist_expected_windspeed) {
			BOOST_TEST_MESSAGE(elem);
			BOOST_CHECK_EQUAL(elem, playlist[i++]);
		}
		BOOST_CHECK_EQUAL(MS_ONE, playlist[i++]);
	}

	if (configuration_file_second->getForecast().locations[0].sayTemperature) {
		BOOST_CHECK_EQUAL(TEMPERATURA, playlist[i++]);
		for (std::string elem : playlist_expected_temperature) {
			BOOST_TEST_MESSAGE(elem);
			BOOST_CHECK_EQUAL(elem, playlist[i++]);
		}
	}

}

BOOST_AUTO_TEST_CASE(current_weather_first) {

	std::shared_ptr<org::openapitools::client::model::QualityFactor> value = std::make_shared<org::openapitools::client::model::QualityFactor>();
	value->setValue(org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_FULL_);

	org::openapitools::client::model::Summary summary;
	summary.setHumidity(12);
	summary.setGusts(7.67f);
	summary.setAverageSpeed(4.00f);
	summary.setDirection(90);
	summary.setAvgTemperature(23.0f);
	summary.setLastTimestamp(TimeTools::getEpoch());
	summary.setTemperatureQf(value);
	summary.setHumidityQf(value);
	summary.setWindQf(value);
	summary.setQnhQf(value);

	auto pair = std::make_pair("skrzyczne", std::make_shared<org::openapitools::client::model::Summary>(summary));

	std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> vector_of_pairs;
	vector_of_pairs.push_back(pair);

	std::vector<std::tuple<std::string, AprsWXData>> weatherlink;

	AprsWXData wx_data;
	wx_data.call = "SR9WXM";
	wx_data.is_primary = true;
	wx_data.valid = true;
	wx_data.useHumidity = true;
	wx_data.usePressure = true;
	wx_data.useTemperature = true;
	wx_data.useWind = true;
	wx_data.packetAgeInSecondsLocal = 5;
	wx_data.packetAgeInSecondsUtc = 5;
	std::vector<AprsWXData> vector_of_wx_data = {wx_data};

	PlaylistAssembler assembler(playlist_sampler, configuration_file_first);

	try {
		assembler.start();

		assembler.currentWeather(vector_of_pairs, vector_of_wx_data, weatherlink);
	}
	catch (...) {
		BOOST_CHECK(false);
	}

	std::shared_ptr<std::vector<std::string>> playlist_ptr = assembler.getPlaylist();
	std::vector<std::string> playlist = *playlist_ptr;

	int i = 0;

	BOOST_CHECK_EQUAL("ident.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(AKTUALNE_WARUNKI, playlist[i++]);
	BOOST_CHECK_EQUAL("magurka.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(KIERUNEK_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_N, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(PORYWY_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TEMPERATURA, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(DEGREE_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(WILGOTNOSC, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);
	BOOST_CHECK_EQUAL("skrzyczne.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(KIERUNEK_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_E, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_4, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_TWO_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(PORYWY_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_7, playlist[i++]);
	BOOST_CHECK_EQUAL(KROPKA, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_6, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TEMPERATURA, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_20, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_3, playlist[i++]);
	BOOST_CHECK_EQUAL(DEGREE_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(WILGOTNOSC, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_12, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);//PERCENTS

}

BOOST_AUTO_TEST_CASE(current_weather_second_with_preanouncement) {

	std::shared_ptr<org::openapitools::client::model::QualityFactor> value = std::make_shared<org::openapitools::client::model::QualityFactor>();
	value->setValue(org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_FULL_);


	org::openapitools::client::model::Summary summary;
	summary.setHumidity(12);
	summary.setGusts(7.67f);
	summary.setAverageSpeed(4.00f);
	summary.setDirection(90);
	summary.setAvgTemperature(23.0f);
	summary.setQnh(999);
	summary.setLastTimestamp(TimeTools::getEpoch());
	summary.setTemperatureQf(value);
	summary.setHumidityQf(value);
	summary.setWindQf(value);
	summary.setQnhQf(value);

	auto pair = std::make_pair("skrzyczne", std::make_shared<org::openapitools::client::model::Summary>(summary));

	std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> vector_of_pairs;
	vector_of_pairs.push_back(pair);

	std::vector<std::tuple<std::string, AprsWXData>> weatherlink;

	AprsWXData wx_data;
	wx_data.call = "SR9WXM";
	wx_data.is_primary = true;
	wx_data.valid = true;
	wx_data.useHumidity = true;
	wx_data.usePressure = true;
	wx_data.useTemperature = true;
	wx_data.useWind = true;
	wx_data.packetAgeInSecondsLocal = 5;
	wx_data.packetAgeInSecondsUtc = 5;
	std::vector<AprsWXData> vector_of_wx_data = {wx_data};
//	std::vector<AprsWXData> vector_of_wx_data;
//	vector_of_wx_data.push_back(wx_data);

	PlaylistAssembler assembler(playlist_sampler, configuration_file_second);

	try {
		assembler.start();

		assembler.recordedAnnouncement(false);		// pre

		assembler.regionalPressure(summary.getQnh());

		assembler.currentWeather(vector_of_pairs, vector_of_wx_data, weatherlink);
	}
	catch (...) {
		BOOST_CHECK(false);
	}

	std::shared_ptr<std::vector<std::string>> playlist_ptr = assembler.getPlaylist();
	auto playlist = *playlist_ptr;


	int i = 0;

	BOOST_CHECK_EQUAL("ident.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL("pre_first.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL("pre_second.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL("pre_third.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(CISNIENIE_REG, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_900, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_90, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_9, playlist[i++]);
	BOOST_CHECK_EQUAL(HECTOPASCALS, playlist[i++]);
	BOOST_CHECK_EQUAL(AKTUALNE_WARUNKI, playlist[i++]);
	BOOST_CHECK_EQUAL("skrzyczne.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(KIERUNEK_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_E, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_4, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_TWO_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(PORYWY_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_7, playlist[i++]);
	BOOST_CHECK_EQUAL(KROPKA, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_6, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TEMPERATURA, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_20, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_3, playlist[i++]);
	BOOST_CHECK_EQUAL(DEGREE_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(WILGOTNOSC, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_12, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);//PERCENTS
	BOOST_CHECK_EQUAL("magurka.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(KIERUNEK_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_N, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(PORYWY_WIATRU, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TEMPERATURA, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(DEGREE_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(WILGOTNOSC, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);
}

BOOST_AUTO_TEST_CASE(gopr_avalanche_levels) {

	AvalancheWarnings_Expositions exposition;
	exposition.north = true;
	exposition.east = true;

	PlaylistAssembler assembler(playlist_sampler, configuration_file_second);

	try {
		assembler.start();

		assembler.avalancheWarning(AvalancheWarnings_Location::BABIA_GORA, 3, exposition);
	}
	catch (...)
	{
		BOOST_CHECK(false);
	}

	std::shared_ptr<std::vector<std::string>> playlist_ptr = assembler.getPlaylist();
	auto playlist = *playlist_ptr;


	int i = 0;

	BOOST_CHECK_EQUAL("ident.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(ZAGROZENIELAWINOWE, playlist[i++]);
	BOOST_CHECK_EQUAL(BABIA, playlist[i++]);
	BOOST_CHECK_EQUAL(TRZECI_ST, playlist[i++]);
	BOOST_CHECK_EQUAL(ESKPOZYCJA, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_N, playlist[i++]);
	BOOST_CHECK_EQUAL(ORAZ, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_E, playlist[i++]);

}


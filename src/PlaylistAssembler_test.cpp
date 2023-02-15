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

	// std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> & forecasts

	// get currrent timestamp
	boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

	// epoch
	boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

	// timestamp
	long ts = (current - epoch).total_seconds();

	//org::openapitools::client::model::Inline_response_200
	auto meteoblueResponse = std::make_shared<org::openapitools::client::model::Inline_response_200>();
	auto data3h = std::make_shared<org::openapitools::client::model::Data_3h>();

	std::vector<int32_t> time;
	std::vector<float> temperature;
	std::vector<int32_t> winddirection;
	std::vector<float> windspeed;

	for (int i = 0; i < 64; i++) {
		time.push_back(ts + i * 1800);
		temperature.push_back(20.0f + 0.1f * i);
		windspeed.push_back(1.0f + 0.1f * i);
		winddirection.push_back(180 + i);
	}

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

	// assemble
	assembler.start();
	BOOST_CHECK_NO_THROW(assembler.forecastMeteoblue(vct));

	std::shared_ptr<std::vector<std::string>> playlist_ptr = assembler.getPlaylist();
	std::vector<std::string> playlist = *playlist_ptr;

	int i = 0;

	BOOST_CHECK_EQUAL("ident.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(FCAST, playlist[i++]);
	BOOST_CHECK_EQUAL(FOR_NEXT, playlist[i++]);

}

BOOST_AUTO_TEST_CASE(current_weather_first) {

	org::openapitools::client::model::Summary summary;
	summary.setHumidity(12);
	summary.setGusts(7.67f);
	summary.setAverageSpeed(4.00f);
	summary.setDirection(90);
	summary.setAvgTemperature(23.0f);

	auto pair = std::make_pair("skrzyczne", std::make_shared<org::openapitools::client::model::Summary>(summary));

	std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> vector_of_pairs;
	vector_of_pairs.push_back(pair);

	AprsWXData wx_data;
	wx_data.call = "SR9WXM";
	wx_data.is_primary = true;
	wx_data.valid = true;
	wx_data.useHumidity = true;
	wx_data.usePressure = true;
	wx_data.useTemperature = true;
	wx_data.useWind = true;
	std::vector<AprsWXData> vector_of_wx_data = {wx_data};

	PlaylistAssembler assembler(playlist_sampler, configuration_file_first);

	try {
		assembler.start();

		assembler.currentWeather(vector_of_pairs, vector_of_wx_data);
	}
	catch (...) {
		BOOST_CHECK(false);
	}

	std::shared_ptr<std::vector<std::string>> playlist_ptr = assembler.getPlaylist();
	std::vector<std::string> playlist = *playlist_ptr;

	int i = 0;

	BOOST_CHECK_EQUAL("ident.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(WEATHER, playlist[i++]);
	BOOST_CHECK_EQUAL("magurka.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_N, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(WINDGUSTS, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TMPRATURE, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(HMIDITY, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);
	BOOST_CHECK_EQUAL("skrzyczne.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_E, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_4, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_TWO_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(WINDGUSTS, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_7, playlist[i++]);
	BOOST_CHECK_EQUAL(DECIMAL, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_6, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TMPRATURE, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_20, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_3, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(HMIDITY, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_12, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);//PERCENTS

}

BOOST_AUTO_TEST_CASE(current_weather_second_with_preanouncement) {

	org::openapitools::client::model::Summary summary;
	summary.setHumidity(12);
	summary.setGusts(7.67f);
	summary.setAverageSpeed(4.00f);
	summary.setDirection(90);
	summary.setAvgTemperature(23.0f);
	summary.setQnh(999);

	auto pair = std::make_pair("skrzyczne", std::make_shared<org::openapitools::client::model::Summary>(summary));

	std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> vector_of_pairs;
	vector_of_pairs.push_back(pair);

	AprsWXData wx_data;
	wx_data.call = "SR9WXM";
	wx_data.is_primary = true;
	wx_data.valid = true;
	wx_data.useHumidity = true;
	wx_data.usePressure = true;
	wx_data.useTemperature = true;
	wx_data.useWind = true;
	std::vector<AprsWXData> vector_of_wx_data = {wx_data};
//	std::vector<AprsWXData> vector_of_wx_data;
//	vector_of_wx_data.push_back(wx_data);

	PlaylistAssembler assembler(playlist_sampler, configuration_file_second);

	try {
		assembler.start();

		assembler.recordedAnnouncement(false);		// pre

		assembler.regionalPressure(summary.getQnh());

		assembler.currentWeather(vector_of_pairs, vector_of_wx_data);
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
	BOOST_CHECK_EQUAL(QNH_REGIONAL, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_900, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_90, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_9, playlist[i++]);
	BOOST_CHECK_EQUAL(HECTOPASCALS, playlist[i++]);
	BOOST_CHECK_EQUAL(WEATHER, playlist[i++]);
	BOOST_CHECK_EQUAL("skrzyczne.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_E, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_4, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_TWO_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(WINDGUSTS, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_7, playlist[i++]);
	BOOST_CHECK_EQUAL(DECIMAL, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_6, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TMPRATURE, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_20, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_3, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(HMIDITY, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_12, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);//PERCENTS
	BOOST_CHECK_EQUAL("magurka.mp3", playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION, playlist[i++]);
	BOOST_CHECK_EQUAL(DIRECTION_N, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(WINDGUSTS, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(MS_FOUR, playlist[i++]);
	BOOST_CHECK_EQUAL(TMPRATURE, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(CELSIUSS, playlist[i++]);
	BOOST_CHECK_EQUAL(HMIDITY, playlist[i++]);
	BOOST_CHECK_EQUAL(NUMBER_0, playlist[i++]);
	BOOST_CHECK_EQUAL(PERCENTS, playlist[i++]);
}

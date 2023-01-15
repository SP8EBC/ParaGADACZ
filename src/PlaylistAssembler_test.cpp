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

ConfigurationFile configuration_file("configuration_playlist_assembler.conf");
std::unique_ptr<PlaylistSampler> playlist_sampler;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/playlistassembler_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
    configuration_file.parse();
    playlist_sampler = std::make_unique<PlaylistSamplerPL>(configuration_file);
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

BOOST_AUTO_TEST_CASE(current_weather_first) {

	org::openapitools::client::model::Summary summary;
	summary.setHumidity(12);
	summary.setGusts(7.67f);
	summary.setAverageSpeed(4.00f);
	summary.setDirection(90);
	summary.setAvgTemperature(23.0f);

	std::pair<std::string, org::openapitools::client::model::Summary> pair = std::make_pair("skrzyczne", summary);

	std::vector<std::pair<std::string, org::openapitools::client::model::Summary>> vector_of_pairs;
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

	PlaylistAssembler assembler(*playlist_sampler, configuration_file);

	try {
		assembler.start();

		assembler.currentWeather(vector_of_pairs, vector_of_wx_data);
	}
	catch (...) {
		BOOST_CHECK(false);
	}

	auto playlist = assembler.getPlaylist();

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

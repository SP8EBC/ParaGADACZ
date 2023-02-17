/*
 * Player_test.cpp
 *
 *  Created on: Jan 24, 2023
 *      Author: mateusz
 */

#include "Player.h"

#define BOOST_TEST_MODULE Player_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <memory>

#include "PlaylistSamplerPL.h"
#include "PlaylistAssembler.h"
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"


ConfigurationFile configuration_file_first("./test_input/configuration_playlist_assembler_first.conf");
ConfigurationFile configuration_file_second("./test_input/configuration_playlist_assembler_second.conf");
std::unique_ptr<PlaylistSampler> playlist_sampler;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/player_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
    configuration_file_first.parse();
    configuration_file_second.parse();
    playlist_sampler = std::make_unique<PlaylistSamplerPL>(configuration_file_first); 	// used for 'getAudioForStationName' and
    																					// 'getAudioForForecast' name

	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);
  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

std::shared_ptr<std::vector<std::string>> getTestPlaylist_1() {
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

	PlaylistAssembler assembler(*playlist_sampler, configuration_file_first);

	try {
		assembler.start();

		assembler.currentWeather(vector_of_pairs, vector_of_wx_data);
	}
	catch (...) {
		return NULL;
	}

	return assembler.getPlaylist();
}

std::shared_ptr<std::vector<std::string>> getTestPlaylist_2() {

	std::shared_ptr<std::vector<std::string>> out = std::make_shared<std::vector<std::string>>();

	out->emplace_back("chirp.mp3");

	return out;
}

std::shared_ptr<std::vector<std::string>> getTestPlaylist_3() {

	std::shared_ptr<std::vector<std::string>> out = std::make_shared<std::vector<std::string>>();

	out->emplace_back("chirp.mp3");
	out->emplace_back("ident.mp3");

	return out;
}

BOOST_GLOBAL_FIXTURE (MyConfig);

//BOOST_AUTO_TEST_CASE(single_file) {
//
//	Player player(configuration_file_first);
//
//	BOOST_CHECK_NO_THROW(player.play(getTestPlaylist_2()));
//
//	BOOST_CHECK_NO_THROW(player.waitForPlaybackToFinish());
//}

BOOST_AUTO_TEST_CASE(two_files) {
	Player player;

	BOOST_CHECK_NO_THROW(player.setPlaylist(getTestPlaylist_3()));

	while (player.playNext()) {
		player.waitForPlaybackToFinish();
	}

	//BOOST_CHECK_NO_THROW(player.waitForPlaybackToFinish());
}

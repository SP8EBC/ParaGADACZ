/*
 * PlaylistCreatorPL_test.cpp
 *
 *  Created on: Dec 31, 2022
 *      Author: mateusz
 */

#include "PlaylistCreatorPL.h"
#include "PlaylistCreatorPL_files.h"


#define BOOST_TEST_MODULE PlaylistCreatorPL_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/playlistcreatorpl_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(decimal_123_4) {

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistCreator> c = std::make_unique<PlaylistCreatorPL>();

	std::vector<std::string> out = c->getAudioListFromNumber(123.4f);

	BOOST_CHECK_EQUAL(5, out.size());
	BOOST_CHECK_EQUAL(NUMBER_100, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_20, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_3, out.at(2));
	BOOST_CHECK_EQUAL(DECIMAL, out.at(3));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(4));

}

BOOST_AUTO_TEST_CASE(decimal_1123_4) {

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistCreator> c = std::make_unique<PlaylistCreatorPL>();

	std::vector<std::string> out = c->getAudioListFromNumber(1123.4f);

	BOOST_CHECK_EQUAL(6, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_100, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_20, out.at(2));
	BOOST_CHECK_EQUAL(NUMBER_3, out.at(3));
	BOOST_CHECK_EQUAL(DECIMAL, out.at(4));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(5));

}

BOOST_AUTO_TEST_CASE(decimal_1103_4) {

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistCreator> c = std::make_unique<PlaylistCreatorPL>();

	std::vector<std::string> out = c->getAudioListFromNumber(1103.4f);

	BOOST_CHECK_EQUAL(5, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_100, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_3, out.at(2));
	BOOST_CHECK_EQUAL(DECIMAL, out.at(3));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(4));

}

BOOST_AUTO_TEST_CASE(decimal_1013_4) {

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistCreator> c = std::make_unique<PlaylistCreatorPL>();

	std::vector<std::string> out = c->getAudioListFromNumber(1013.4f);

	BOOST_CHECK_EQUAL(4, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_13, out.at(1));
	BOOST_CHECK_EQUAL(DECIMAL, out.at(2));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(3));

}

BOOST_AUTO_TEST_CASE(decimal_1013) {

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistCreator> c = std::make_unique<PlaylistCreatorPL>();

	std::vector<std::string> out = c->getAudioListFromNumber(1013.0f);

	BOOST_CHECK_EQUAL(2, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_13, out.at(1));

}

BOOST_AUTO_TEST_CASE(decimal_999) {

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistCreator> c = std::make_unique<PlaylistCreatorPL>();

	std::vector<std::string> out = c->getAudioListFromNumber(999.0f);

	BOOST_CHECK_EQUAL(3, out.size());
	BOOST_CHECK_EQUAL(NUMBER_900, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_90, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_9, out.at(2));

}

BOOST_AUTO_TEST_CASE(decimal_11) {

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistCreator> c = std::make_unique<PlaylistCreatorPL>();

	std::vector<std::string> out = c->getAudioListFromNumber(11.0f);

	BOOST_CHECK_EQUAL(1, out.size());
	BOOST_CHECK_EQUAL(NUMBER_11, out.at(0));


}


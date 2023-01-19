/*
 * ForecastDownloader_test.cpp
 *
 *  Created on: Jan 17, 2023
 *      Author: mateusz
 */

#define BOOST_TEST_MODULE ForecastDownloader_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include "ForecastDownloader.h"

ConfigurationFile config("./test_input/configuration_forecast_downloader.conf");

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/forecastdownloader_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
    config.parse();
  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(download_first_set)
{

	BOOST_TEST_MESSAGE("creating downloader instance");

	ForecastDownloader downloader(config);

	BOOST_TEST_MESSAGE("downloading meteoblue");

	downloader.downloadAllMeteoblue();

	BOOST_CHECK(true);
}

/*
 * ForecastDownloader_test.cpp
 *
 *  Created on: Jan 17, 2023
 *      Author: mateusz
 */

#include "ForecastDownloader.h"

#define BOOST_TEST_MODULE ForecastDownloader_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

ConfigurationFile config("./test_input/configuration_playlist_creator_pl.conf");

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/forecastdownloader_test.log" )
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


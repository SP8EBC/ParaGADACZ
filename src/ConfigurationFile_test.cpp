/*
 * ConfigurationFile_test.cpp
 *
 *  Created on: Dec 24, 2022
 *      Author: mateusz
 */

#include "ConfigurationFile.h"

#define BOOST_TEST_MODULE ConfigurationFile_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/configuraionfile_test.log" )
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

BOOST_AUTO_TEST_CASE (simple_parse)
{
	ConfigurationFile configurationFile("configuration.conf");

	spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [%s:%#] %v" );

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);
}


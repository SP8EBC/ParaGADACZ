/*
 * TimeTools_test.cpp
 *
 *  Created on: Dec 29, 2023
 *      Author: mateusz
 */

#include "TimeTools.h"

#define BOOST_TEST_MODULE TimeTools_test
#include <boost/test/included/unit_test.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


struct MyConfig
{
  MyConfig() : test_log( "./test_reports/timetools_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(test_getTmStructFromUndefinedTzEpoch)
{
	const unsigned long timestamp = 1703875348ULL;
	// 1703875348   - Friday, 29 December 2023 18:42:28
	tm expected = {0u};

	expected.tm_hour = 18;
	expected.tm_min = 42;
	expected.tm_sec = 28;
	expected.tm_mday = 29;
	expected.tm_mon = 11;		// minus jeden!
	expected.tm_year = 123;

	tm given  = TimeTools::getTmStructFromUndefinedTzEpoch(timestamp);

	BOOST_CHECK_EQUAL(expected.tm_hour, given.tm_hour);
	BOOST_CHECK_EQUAL(expected.tm_min, given.tm_min);
	BOOST_CHECK_EQUAL(expected.tm_sec, given.tm_sec);
	BOOST_CHECK_EQUAL(expected.tm_mday, given.tm_mday);
	BOOST_CHECK_EQUAL(expected.tm_mon, given.tm_mon);
	BOOST_CHECK_EQUAL(expected.tm_year, given.tm_year);

}

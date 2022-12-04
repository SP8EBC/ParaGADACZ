/*
 * AprxLogParser_test.cpp
 *
 *  Created on: Dec 3, 2022
 *      Author: mateusz
 */

#include "AprxLogParser.h"

#define BOOST_TEST_MODULE AprxLogParser_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/aprxlogparser_test.log" )
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

BOOST_AUTO_TEST_CASE (one)
{
	const std::string expected = "2022-12-03 14:05:08.760 SR9NSK    R SR9NWC>APMI03:=4939.19NL01959.53E#PHG2620 W3,SPn 80km Lubon W. alt=1040 Jerzy SP9FBT";

	AprxLogParser parser("aprs-rf-2.log");

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NWC", 0);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

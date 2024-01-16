/*
 * PansaAirspace_test.cpp
 *
 *  Created on: Jan 3, 2024
 *      Author: mateusz
 */

#include "PansaAirspace.h"

#define BOOST_TEST_MODULE PansaAirspace_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

#include "PansaAirspace.h"

// user=postgres host=localhost password=dupajasia123 dbname=airspace
#define PSQL_USER 		"postgres"
#define PSQL_PASSWORD	"dupajasia123"
#define PSQL_DB			"airspace"

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/PansaAirspace_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(connect)
{
	PansaAirspace pansa(PSQL_USER, PSQL_PASSWORD, PSQL_DB);

	//pansa.connect();
	//pansa.downloadAroundLocation(49.6852, 19.0318, 70000.0f, true);
	pansa.downloadForDesginator("EPTS6A", true);
}

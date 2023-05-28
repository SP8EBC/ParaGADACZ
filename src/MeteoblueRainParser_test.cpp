/*
 * MeteoblueRainParser_test.cpp
 *
 *  Created on: May 8, 2023
 *      Author: mateusz
 */

#define BOOST_TEST_MODULE MeteoblueRainParser_test
#include <boost/test/included/unit_test.hpp>

#include "MeteoblueRainParser.h"


#include <fstream>

#include <memory>
#include <utility>

class Test : MeteoblueRainParser {

public:

	static MeteoblueRainParser::MeteoblueRainParser_PrecipType parse(
			MeteobluePictocode pictocode,
			std::string rainspot)
	{
		return MeteoblueRainParser::parse(pictocode, rainspot);
	}
};

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/meteobluerainparser_test.log" )
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

BOOST_AUTO_TEST_CASE(first)
{
	const std::string input = "0011211222211122221120012211100021110001121000222";
	const MeteobluePictocode pict = MeteobluePictocode::MIXED_WITH_SHOWERS;

	MeteoblueRainParser::MeteoblueRainParser_PrecipType value = Test::parse(pict, input);

	BOOST_CHECK_EQUAL(MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_MEDIUM_RAIN, value);
}

BOOST_AUTO_TEST_CASE(second)
{
	const std::string input = "0011211222211123331120012211100031110001131000222";
	const MeteobluePictocode pict = MeteobluePictocode::LIGHT_RAIN_THUNDERSTORMS_LIKELY;

	MeteoblueRainParser::MeteoblueRainParser_PrecipType value = Test::parse(pict, input);

	BOOST_CHECK_EQUAL(MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_THUNDERSTORM, value);
}

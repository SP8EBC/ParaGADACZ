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

BOOST_AUTO_TEST_CASE (split_callsign)
{
	std::string test = "SP8EBC-11";

	std::tuple<std::string, unsigned> result = ConfigurationFile::splitCallsign(test);

	BOOST_CHECK_EQUAL(std::get<std::string>(result), "SP8EBC");
	BOOST_CHECK_EQUAL(std::get<unsigned>(result), 11);
}

BOOST_AUTO_TEST_CASE (split_callsign_no_ssid)
{
	std::string test = "SP8EBC";

	std::tuple<std::string, unsigned> result = ConfigurationFile::splitCallsign(test);

	BOOST_CHECK_EQUAL(std::get<std::string>(result), "SP8EBC");
	BOOST_CHECK_EQUAL(std::get<unsigned>(result), 0);
}

BOOST_AUTO_TEST_CASE (inhibitor_parse)
{
	ConfigurationFile configurationFile("./test_input/configuration_inhibitor_parse.conf");

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);
	/////////////////////////////////////
	const ConfigurationFile_Inhibitor inhibitor = configurationFile.getInhibitor();
	BOOST_CHECK_EQUAL(inhibitor.serial.enable, true);
	BOOST_CHECK_EQUAL(inhibitor.serial.okActiveLevel, false);
	BOOST_CHECK_EQUAL(inhibitor.serial.port, "/dev/ttyS0");

	BOOST_CHECK_EQUAL(inhibitor.http.enable, true);
	BOOST_CHECK_EQUAL(inhibitor.http.ignoreNoAnswer, true);
	BOOST_CHECK_EQUAL(inhibitor.http.url, "http://pogoda.cc");

	BOOST_CHECK_EQUAL(inhibitor.exec.enable, true);
	BOOST_CHECK_EQUAL(inhibitor.exec.okRetval, 0);
	BOOST_CHECK_EQUAL(inhibitor.exec.path, "/bin/more");
	////////////////////////////////////////
//	const ConfigurationFile_Avalanche avalanceWarning = configurationFile.getAvalancheWarning();
//	BOOST_CHECK_EQUAL(avalanceWarning.goprBabiaGora, true);
}

BOOST_AUTO_TEST_CASE (simple_parse)
{
	ConfigurationFile configurationFile("./test_input/configuration_simple_parse.conf");

	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.getIntro().ident.at(0), "intro.ogg");
	BOOST_CHECK_EQUAL(configurationFile.getIntro().sayCurrentTime, false);
	BOOST_CHECK_EQUAL(configurationFile.getIntro().sayRegionalPressure, true);
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.isDebug(), false);
	BOOST_CHECK_EQUAL(configurationFile.getLogOutput(), "gadacz.log");
	BOOST_CHECK_EQUAL(configurationFile.getMaximumDataAgeInMinutes(), 60);
	BOOST_CHECK_EQUAL(configurationFile.getAprxConfig().aprxRfLogPath, "/home/mateusz/Documents/test/aprx/aprx-rf.log");
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().size(), 2);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).name, "SR9WXM");
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).type, APRX);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).fnIdent, "magurka.mp3");
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayHumidy, true);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayTemperature, true);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayWind, true);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayPressure, false);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).regionalPressure, true);
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.getForecast().futureTime , 180);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.size(), 2);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).name, "Skrzyczne");
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).nameIdent, "costam.mp3");
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).sayPrecipation, true);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).sayTemperature, true);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).sayWind, true);


}


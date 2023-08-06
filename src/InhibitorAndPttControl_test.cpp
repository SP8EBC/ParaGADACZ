/*
 * InhibitorAndPttControl_test.cpp
 *
 *  Created on: Jul 10, 2023
 *      Author: mateusz
 */


#define BOOST_TEST_MODULE InhibitorAndPttControl_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

#include "InhibitorAndPttControl.h"
#include "ConfigurationFile.h"

ConfigurationFile_PttControl config_parport, config_lp;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/InhibitorAndPttControl_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);


	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	config_parport.pttSerialportDevice = "";
	config_parport.pttParportDevice = "/dev/parport0";
	config_parport.pttParportLinesSelector = 0xCCu;
	//config_parport.parportPtt = true;
	config_parport.postAnounmntDelay = 4444;
	config_parport.preAnounmntDelay = 4444;

	config_lp.pttSerialportDevice = "";
	config_lp.pttParportDevice = "/dev/lp0";
	config_lp.pttParportLinesSelector = 0xCCu;
	//config_lp.parportPtt = true;
	config_lp.preAnounmntDelay = 4444;

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(parport)
{
	InhibitorAndPttControl ptt;

	BOOST_CHECK(ptt.setConfigAndCheckPort(config_parport));
	ptt.keyPtt(true);
	ptt.dekeyPtt();
}

BOOST_AUTO_TEST_CASE(lpzero)
{
	InhibitorAndPttControl ptt;

	BOOST_CHECK(ptt.setConfigAndCheckPort(config_lp));
	ptt.keyPtt(true);
	ptt.dekeyPtt();
}

/*
 * WeatherlinkDownloader_test.cpp
 *
 *  Created on: Mar 19, 2023
 *      Author: mateusz
 */

#define BOOST_TEST_MODULE WeatherlinkDownloader_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include "WeatherlinkDownloader.h"
#include "secret.h"

std::shared_ptr<ConfigurationFile> config;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/weatherlinkdownloader_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
    config = std::make_shared<ConfigurationFile>("./test_input/configuration_forecast_downloader.conf");
    config->parse();
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
	WeatherlinkDownloader weatherlink(config);

	weatherlink.downloadForStation("001D0A00C5C3", WEATHERLINK_PASS , WEATHERLINK_TOKEN);
	auto result = weatherlink.getDownloadedContent();
	AprsWXData wxData = WeatherlinkDownloader::convertModelToWxData(result);
	utility::string_t credit = result->getCredit();

	BOOST_CHECK_EQUAL(credit, "Davis Instruments Corp.");
	BOOST_CHECK_EQUAL(wxData.call, "bspzar");
}

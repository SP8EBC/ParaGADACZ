/*
 * TrendDownloader_test.cpp
 *
 *  Created on: Aug 18, 2023
 *      Author: mateusz
 */

#define BOOST_TEST_MODULE TrendDownloader_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"]

#include "TrendDownloader.h"
#include "PogodaccDownloader.h"

std::shared_ptr<PogodaccDownloader> pogodaccDownloader;

std::shared_ptr<ConfigurationFile> config;


struct MyConfig
{
  MyConfig() : test_log( "./test_reports/trenddownloader_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

    config = std::make_shared<ConfigurationFile>("./test_input/configuration_forecast_downloader.conf");
    config->parse();

	pogodaccDownloader = std::make_shared<PogodaccDownloader>(config);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

/***
 * 	static int downloadTrendData(std::vector<TrendDownloader_Data> & out,
								const ConfigurationFile & config,
								std::shared_ptr<org::openapitools::client::api::StationApi> stationApi,
								AprxLogParser & logParser);
 */

BOOST_AUTO_TEST_CASE(pogodacc_skrzyczne)
{
	std::vector<TrendDownloader_Data> trend;

	AprxLogParser parser;

	TrendDownloader::downloadTrendData(trend, *config, pogodaccDownloader->getStationApi(), parser);


}

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
#include "spdlog/sinks/stdout_color_sinks.h"

#include "TrendDownloader.h"
#include "PogodaccDownloader.h"
#include "TimeTools.h"

std::shared_ptr<PogodaccDownloader> pogodaccDownloader;

std::shared_ptr<ConfigurationFile> config_skrzyczne, config_jezioro;


struct MyConfig
{
  MyConfig() : test_log( "./test_reports/trenddownloader_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

	config_jezioro = std::make_shared<ConfigurationFile>("./test_input/configuration_trend_downloader_jezioro.conf");
	config_jezioro->parse();

	config_skrzyczne = std::make_shared<ConfigurationFile>("./test_input/configuration_trend_downloader_skrzyczne.conf");
	config_skrzyczne->parse();

	pogodaccDownloader = std::make_shared<PogodaccDownloader>(config_skrzyczne);

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

//BOOST_AUTO_TEST_CASE(pogodacc_skrzyczne)
//{
//	TrendDownloader_GetTime<TimeTools> t;
//
//	int64_t current = t.getTime();
//
//	std::vector<TrendDownloader_Data> trend;
//
//	AprxLogParser parser;
//
//	TrendDownloader::downloadTrendData(trend, *config_skrzyczne, pogodaccDownloader->getStationApi(), parser, current);
//
//
//}

BOOST_AUTO_TEST_CASE(aprx_log_1)
{

	/**
	 * Date and time (GMT): Saturday, 26 August 2023 06:00:00
	 * Date and time (your time zone): Saturday, 26 August 2023 08:00:00 GMT+02:00
	 */
	int64_t current = 1693029600;

	// trend data
	std::vector<TrendDownloader_Data> trend;

	// aprx log parser
	AprxLogParser parser("./test_input/aprx-rf_trend_test.log", true);
	parser.openFile();

	/**
	 * trend between
	 * 	GMT: Saturday, 26 August 2023 03:40:00
		Your time zone: Saturday, 26 August 2023 05:40:00 GMT+02:00 DST
	 * and
	 * 	GMT: Saturday, 26 August 2023 04:20:00
	 *	Your time zone: Saturday, 26 August 2023 06:20:00 GMT+02:00 DST
	 *
	 */

	/**
	 * 2023-08-26 05:43:17.341 SR9NSK-4  R SR9WXZ>AKLPRZ:!4943.43N/01912.10E_214/004g004t073r...p...P...b00000h00	1.75999999
	 * 2023-08-26 05:47:17.278 SR9NSK-4  R SR9WXZ>AKLPRZ:!4943.43N/01912.10E_215/009g011t073r...p...P...b00000h00	3.96000004
	 * 2023-08-26 05:59:20.529 SR9NSK-4  R SR9WXZ>AKLPRZ:!4943.43N/01912.10E_216/004g007t073r...p...P...b00000h00	1.75999999
	 * 2023-08-26 06:11:24.763 SR9NSK-4  R SR9WXZ>AKLPRZ:!4943.43N/01912.10E_217/003g008t073r...p...P...b00000h00	1.32000005
	 *
	 *
	 */

	const int stations = TrendDownloader::downloadTrendData(trend, *config_jezioro, pogodaccDownloader->getStationApi(), parser, current);

	BOOST_CHECK_EQUAL(1, stations);
	BOOST_CHECK_EQUAL(1, trend.size());

	auto trendElement = trend.at(0);

	BOOST_CHECK_EQUAL(2, trendElement.windspeedTrend);

}

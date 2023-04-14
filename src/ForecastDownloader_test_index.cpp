/*
 * ForecastDownloader_test_index.cpp
 *
 *  Created on: Apr 14, 2023
 *      Author: mateusz
 */


#define BOOST_TEST_MODULE ForecastDownloader_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include "ForecastDownloader.h"

std::shared_ptr<ConfigurationFile> config;

class ForecastDownloader_test : ForecastDownloader {

public:

	ForecastDownloader_test(std::shared_ptr<ConfigurationFile> & config) : ForecastDownloader(config) {

	}

	bool _loadCacheIndex() {
		return loadCacheIndex();
	}

	auto _getCacheIndex() {
		return getCacheIndex();
	}
};

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/forecastdownloader_test_index.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
    config = std::make_shared<ConfigurationFile>("./test_input/configuration_forecast_index.conf");
    config->parse();
  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);


BOOST_AUTO_TEST_CASE(first_good_idx)
{

	ForecastDownloader_test downloader(config);

	BOOST_CHECK_EQUAL(true, downloader._loadCacheIndex());

	std::map<std::string, ForecastDownloader_CacheIndexElem> _map = downloader._getCacheIndex();

	BOOST_CHECK_EQUAL(3, _map.size());
}


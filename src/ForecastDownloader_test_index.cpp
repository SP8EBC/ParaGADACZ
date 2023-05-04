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

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<ConfigurationFile> config, config2, save_cache;

class ForecastDownloader_test : public ForecastDownloader {

public:

	ForecastDownloader_test(std::shared_ptr<ConfigurationFile> & config) : ForecastDownloader(config) {

	}

	bool _loadCacheIndex() {
		return loadCacheIndex();
	}

	auto _getCacheIndex() {
		return getCacheIndex();
	}

	auto _saveCacheIndex(std::map<std::string, ForecastDownloader_CacheIndexElem> & idx) {
		return saveCacheIndex(idx);
	}

	auto _saveInCache(std::string & data, std::string & name) {
		return saveInCache(data, name);
	}

	std::shared_ptr<org::openapitools::client::model::Inline_response_200> _loadFromCache (std::string name) {
		return loadFromCache(name);
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

    config2 = std::make_shared<ConfigurationFile>("./test_input/configuration_forecast_index_output.conf");
    config2->parse();

    save_cache = std::make_shared<ConfigurationFile>("./test_input/configuration_forecast_downloader_save_cache.conf");
    save_cache->parse();

	spdlog::set_level(spdlog::level::debug);
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

BOOST_AUTO_TEST_CASE(create_index)
{
	ForecastDownloader_test downloader(config2), reader(config2);

	// test map to save into disk
	std::map<std::string, ForecastDownloader_CacheIndexElem> _map;

	ForecastDownloader_CacheIndexElem first, second, third;
	first.filename = "dupa";
	first.locationName = "aaa";
	first.timestamp = 12345;
	second.filename = "dupa2";
	second.locationName = "bbb";
	second.timestamp = 5678;
	third.filename = "dupa3";
	third.locationName = "ccc";
	third.timestamp = 9087;

	_map.insert(std::pair{"dupa", first});
	_map.insert(std::pair{"dupa2", second});
	_map.insert(std::pair{"dupa3", third});

	BOOST_CHECK_EQUAL(true, downloader._saveCacheIndex(_map));

	BOOST_CHECK_EQUAL(true, reader._loadCacheIndex());
	BOOST_CHECK_EQUAL(3, reader._getCacheIndex().size());

}

BOOST_AUTO_TEST_CASE(download_and_save)
{
	/**
	 * Enable this test to download real forecast for skrzyczne
	 * and store it into "./test_output/test_cache_save/" directory
	 */
	ForecastDownloader_test downloader(save_cache);

	downloader.downloadAllMeteoblue();
}

BOOST_AUTO_TEST_CASE(read_from_cache)
{
	std::shared_ptr<org::openapitools::client::model::Inline_response_200> skrzyczne_result;

	ForecastDownloader_test downloader(config);

	downloader._loadCacheIndex();

	BOOST_CHECK_NO_THROW(skrzyczne_result = downloader._loadFromCache("Skrzyczne"));

	std::vector<float>& temperature = skrzyczne_result->getData3h()->getTemperature();

	BOOST_CHECK_CLOSE(4.07f, temperature.at(0), 1);

}

/*
 * EmailDownloader_test.cpp
 *
 *  Created on: Sep 1, 2023
 *      Author: mateusz
 */

#include "EmailDownloader.h"
#include "EmailDownloader_test_secret.h"

#define BOOST_TEST_MODULE EmailDownloader_test
#include <boost/test/included/unit_test.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

ConfigurationFile_Email configEmail;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/emaildownloader_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

    configEmail.serverConfig.pop3Address = "poczta.interia.pl";
    configEmail.serverConfig.pop3Port = 995;
    configEmail.serverConfig.imapAddress = "poczta.interia.pl";
    configEmail.serverConfig.imapPort = 993;
    configEmail.serverConfig.username = USERNAME;
    configEmail.serverConfig.password = PASSWD;
    configEmail.serverConfig.startTls = false;

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
	EmailDownloader downloader(configEmail);

	BOOST_CHECK(downloader.downloadAllEmailsImap() > 0);
}


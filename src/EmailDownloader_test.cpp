/*
 * EmailDownloader_test.cpp
 *
 *  Created on: Sep 1, 2023
 *      Author: mateusz
 */

#include "EmailDownloader.h"
#include "EmailDownloader_test_secret.h"

#include "TimeTools.h"

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

	ConfigurationFile_Email_AllowedSender sender, sender2;
	sender.emailAddress = "sklep@8a.pl";
	sender2.emailAddress = "maciej.brylski@corporatenexus.pl";


    configEmail.serverConfig.pop3Address = "poczta.interia.pl";
    configEmail.serverConfig.pop3Port = 995;
    configEmail.serverConfig.imapAddress = "poczta.interia.pl";
    configEmail.serverConfig.imapPort = 993;
    configEmail.serverConfig.username = USERNAME;
    configEmail.serverConfig.password = PASSWD;
    configEmail.serverConfig.startTls = false;

    configEmail.allowedSendersList.push_back(sender);
    configEmail.allowedSendersList.push_back(sender2);

    TimeTools::initBoostTimezones();

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

//BOOST_AUTO_TEST_CASE(first)
//{
//	/**
//	 * 	Default:"------=_Part_17674_772542156.1693548324914\r\nContent-Type: text/plain; charset=UTF-8\r\nContent-Transfer-Encoding: quoted-printable\r\n\r\nDokument zosta=C5=82 podpisany 01.09.2023 roku o godzinie 08:04 prze"...
//	 *
//	 *
//	 *
//	 */
//
//	EmailDownloader downloader(configEmail);
//
//	BOOST_CHECK(downloader.downloadAllEmailsImap() > 0);
//
//	std::vector<EmailDownloaderMessage> messages = downloader.getMessages();
//	BOOST_CHECK_GT(messages.size(), 0);
//
//	for (EmailDownloaderMessage msg : messages) {
//		BOOST_TEST_MESSAGE(msg.getContent());
//	}
//}

BOOST_AUTO_TEST_CASE(second)
{
	/**
	 *
	 * const std::vector<std::locale> EmailDownloader::formats(
	{
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%m/%d/%Y %H:%M")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%m/%d/%Y %I:%M%p")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%b %d, %Y %I:%M"))
	}
);
	 */

		std::string test = "until October 23, 2023 11:22";

		// 1698019200

		BOOST_CHECK(EmailDownloader::validateEmailSubject(test, configEmail.allowedSendersList.at(0)));

}

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

	ConfigurationFile_Email_AllowedSender sender, sender2, sender3;
	sender.emailAddress = "sklep@8a.pl";
	sender.eodAnnouncement = true;
	sender.singleAnnouncement = true;
	sender.timedAnnouncement = true;
	sender2.emailAddress = "noreply@bandcamp.com";
	sender2.timedAnnouncement = true;
	sender3.emailAddress = "maciej.brylski@corporatenexus.pl";
	sender3.timedAnnouncement = false;

    configEmail.serverConfig.pop3Address = "poczta.interia.pl";
    configEmail.serverConfig.pop3Port = 995;
    configEmail.serverConfig.imapAddress = "poczta.interia.pl";
    configEmail.serverConfig.imapPort = 993;
    configEmail.serverConfig.username = USERNAME;
    configEmail.serverConfig.password = PASSWD;
    configEmail.serverConfig.startTls = false;

    configEmail.allowedSendersList.push_back(sender);
    configEmail.allowedSendersList.push_back(sender2);
    configEmail.allowedSendersList.push_back(sender3);

    TimeTools::initBoostTimezones();

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
	/**
	 * 	Default:"------=_Part_17674_772542156.1693548324914\r\nContent-Type: text/plain; charset=UTF-8\r\nContent-Transfer-Encoding: quoted-printable\r\n\r\nDokument zosta=C5=82 podpisany 01.09.2023 roku o godzinie 08:04 prze"...
	 *
	 *
	 *
	 */

	EmailDownloader downloader(configEmail);

	BOOST_CHECK(downloader.downloadAllEmailsImap() > 0);

	std::vector<EmailDownloaderMessage> messages = downloader.getMessages();
	BOOST_CHECK_GT(messages.size(), 0);

	for (EmailDownloaderMessage msg : messages) {
		BOOST_TEST_MESSAGE(msg.getContent());
	}
}

BOOST_AUTO_TEST_CASE(validateEmailSubject)
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
		std::string test2 = "until 09/09/2023 11:22";

		// 1698019200

		BOOST_CHECK(std::get<0>(EmailDownloader::validateEmailSubject(test2, configEmail.allowedSendersList.at(0))));
		BOOST_CHECK(std::get<0>(EmailDownloader::validateEmailSubject(test2, configEmail.allowedSendersList.at(2))) == false);

}


BOOST_AUTO_TEST_CASE(validateEmailAgainstPrivileges)
{
	std::string address2("noreply@bandcamp.com");
	std::string address("sklep@8a.pl");
	std::string topic2 = "until 09/09/2023 11:22";
	std::string emailDispatchDateTime = "Wed, 6 Sep 2023 13:56:31 +0000";
	uint64_t emailDispatchUtcTimestamp = 1694008591ULL;
	uint64_t emailReceiveUtcTimestmp = 1694533288ULL;
	std::string originalEncoding = "7bit";
	std::string originalCharset = "us-ascii";

	tm datetime;

	// copy date time into tm structure
	datetime.tm_year = 2023;
	datetime.tm_mon = 9;
	datetime.tm_mday = 6;
	datetime.tm_hour = 13;
	datetime.tm_min = 56;
	datetime.tm_sec = 31;

	// convert that tm to have dispatch time&date in boost local_date_time format
	boost::local_time::local_date_time emailDispatchBoostDate =
			TimeTools::getLocalTimeFromTmStructAndTzOffset(datetime, true, GMT);

	EmailDownloaderMessage msg1(
							address,
							topic2,
							emailDispatchDateTime,
							emailDispatchBoostDate,
							0,
							emailDispatchUtcTimestamp,
							emailReceiveUtcTimestmp,
							"123",
							"123",
							originalEncoding,
							originalCharset);

	EmailDownloaderMessage msg2(
							address2,
							topic2,
							emailDispatchDateTime,
							emailDispatchBoostDate,
							0,
							emailDispatchUtcTimestamp,
							emailReceiveUtcTimestmp,
							"123",
							"123",
							originalEncoding,
							originalCharset);

	EmailDownloader downloader(configEmail);

	std::vector<EmailDownloaderMessage> messages;
	messages.push_back(msg1);
	messages.push_back(msg2);

	downloader.validateEmailAgainstPrivileges(messages);

	BOOST_CHECK(messages.at(0).isValidated());
	BOOST_CHECK(messages.at(1).isValidated());

	BOOST_CHECK_EQUAL(1694258520ULL, messages.at(0).getValidUntil());
	BOOST_CHECK_EQUAL(1694258520ULL, messages.at(1).getValidUntil());

}

BOOST_AUTO_TEST_CASE(validateSingleShot)
{
	std::string address2("noreply@bandcamp.com");
	std::string address("sklep@8a.pl");
	std::string topic2 = "single";
	std::string emailDispatchDateTime = "Wed, 6 Sep 2023 13:56:31 +0000";
	uint64_t emailDispatchUtcTimestamp = 1694008591ULL;
	uint64_t emailReceiveUtcTimestmp = 1694533288ULL;
	std::string originalEncoding = "7bit";
	std::string originalCharset = "us-ascii";

	tm datetime;

	// copy date time into tm structure
	datetime.tm_year = 2023;
	datetime.tm_mon = 9;
	datetime.tm_mday = 20;
	datetime.tm_hour = 13;
	datetime.tm_min = 56;
	datetime.tm_sec = 31;

	// convert that tm to have dispatch time&date in boost local_date_time format
	boost::local_time::local_date_time emailDispatchBoostDate =
			TimeTools::getLocalTimeFromTmStructAndTzOffset(datetime, true, GMT);

	EmailDownloaderMessage msg1(
							address,
							topic2,
							emailDispatchDateTime,
							emailDispatchBoostDate,
							0,
							emailDispatchUtcTimestamp,
							emailReceiveUtcTimestmp,
							"123",
							"123",
							originalEncoding,
							originalCharset);

	EmailDownloader downloader(configEmail);

	std::vector<EmailDownloaderMessage> messages;
	messages.push_back(msg1);

	const int amountEmailsValidated = downloader.validateEmailAgainstPrivileges(messages);
	BOOST_CHECK_EQUAL(amountEmailsValidated, 1);

	BOOST_CHECK_EQUAL(messages.at(0).getValidUntil(), 666ULL);
	BOOST_CHECK_EQUAL(messages.at(0).getEmailReceiveUtcTimestmp(), emailReceiveUtcTimestmp);

}

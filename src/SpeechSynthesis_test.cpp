/*
 * SpeechSynthesis_test.cpp
 *
 *	This tests checks only methods implemented at base class, universal
 *	across specific implementation
 *
 *  Created on: Sep 18, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesisResponsivevoice.h"
#include "EmailDownloader.h"
#include "TimeTools.h"

#define BOOST_TEST_MODULE SpeechSynthesis_test
#include <boost/test/included/unit_test.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/speechsynthesis_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(readIndex)
{
	SpeechSynthesisResponsivevoice synth("kvfbSITh");

	const std::string fn = "./test_input/ttsIndex1.json";

	int result;

	BOOST_CHECK_NO_THROW(result = synth.readIndex(fn));
	BOOST_CHECK_EQUAL(2, result);

	const std::list<SpeechSynthesis_MessageIndexElem>& index = synth.getIndexContent();
	std::list<SpeechSynthesis_MessageIndexElem>::const_iterator it = index.begin();

	BOOST_CHECK_EQUAL("test1.mp3", it->filename);
	BOOST_CHECK_EQUAL(12345, it->sayUntil);
	BOOST_CHECK_EQUAL("test@interia.pl", it->sender);
	BOOST_CHECK_EQUAL(12340, it->receivedAt);

	BOOST_CHECK_NO_THROW(it++);

	BOOST_CHECK_EQUAL("test2.mp3", it->filename);
	BOOST_CHECK_EQUAL(54321, it->sayUntil);
	BOOST_CHECK_EQUAL("test@gmail.pl", it->sender);
	BOOST_CHECK_EQUAL(54300, it->receivedAt);
}

BOOST_AUTO_TEST_CASE(validateSingleShot)
{
	ConfigurationFile_Email configEmail;

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
    configEmail.serverConfig.username = "dupa";
    configEmail.serverConfig.password = "dupa123";
    configEmail.serverConfig.startTls = false;

    configEmail.allowedSendersList.push_back(sender);
    configEmail.allowedSendersList.push_back(sender2);
    configEmail.allowedSendersList.push_back(sender3);

    TimeTools::initBoostTimezones();

	std::string address2("noreply@bandcamp.com");
	std::string address(sender.emailAddress);
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

	SpeechSynthesisResponsivevoice synth("kvfbSITh");

	const std::string fn = "./test_input/ttsIndex2.json";

	BOOST_CHECK_NO_THROW(synth.createIndex(fn));
	BOOST_CHECK_EQUAL(synth.getIndexContent().size(), 0);

	synth.convertEmailsToSpeech(messages, 0, SPEECH_POLISH);
	BOOST_CHECK_EQUAL(synth.getIndexContent().size(), 1);

	BOOST_CHECK_NO_THROW(synth.storeIndex());

	SpeechSynthesisResponsivevoice synthRead("xxxxx2");
	BOOST_CHECK_NO_THROW(synthRead.readIndex(fn));
	BOOST_CHECK_EQUAL(synthRead.getIndexContent().size(), 1);

	const std::list<SpeechSynthesis_MessageIndexElem>::const_iterator index = synthRead.getIndexContent().begin();
	const SpeechSynthesis_MessageIndexElem & firstElem = *index;

	BOOST_CHECK_EQUAL(firstElem.sayUntil, 666ULL);
	BOOST_CHECK_EQUAL(firstElem.sender, sender.emailAddress);
	BOOST_CHECK_EQUAL(firstElem.receivedAt, emailReceiveUtcTimestmp);
}


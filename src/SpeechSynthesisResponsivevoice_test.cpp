/*
 * SpeechSynthesisResponsivevoice_test.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */


#include "SpeechSynthesisResponsivevoice.h"

#include "TimeTools.h"

#define BOOST_TEST_MODULE SpeechSynthesisResponsivevoice_test
#include <boost/test/included/unit_test.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

BOOST_AUTO_TEST_CASE(convertToSpeech_checkMd5)
{
	SpeechSynthesisResponsivevoice synth("kvfbSITh");

	// vector with email messages
	std::vector<EmailDownloaderMessage> msgs;

	// message content
	std::string address2("noreply@bandcamp.com");
	std::string address("sklep@8a.pl");
	std::string topic2 = "until 09/09/2023 11:22";
	std::string emailDispatchDateTime = "Wed, 6 Sep 2023 13:56:31 +0000";
	uint64_t emailDispatchUtcTimestamp = 1694008591ULL;
	uint64_t emailReceiveUtcTimestmp = 1694533288ULL;
	std::string originalEncoding = "7bit";
	std::string originalCharset = "us-ascii";

	tm datetime;

	std::string content = "alamakotaakotmaale";
	std::string contentmd5 = "988C140B7CFE18F4F0562467F27EC5CD";

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
							content,
							content,
							originalEncoding,
							originalCharset);

	msgs.push_back(msg1);

	synth.convertEmailsToSpeech(msgs, 0u);

	auto indexElem = synth.getIndexContent().begin();

	BOOST_CHECK_EQUAL(contentmd5, indexElem->filename);
}

BOOST_AUTO_TEST_CASE(loadIndex)
{
	SpeechSynthesisResponsivevoice synth("kvfbSITh");

	std::string index_filename = "./test_input/";

	BOOST_CHECK(synth.readIndex(index_filename));
}

BOOST_AUTO_TEST_CASE(second)
{
	SpeechSynthesisResponsivevoice synth("kvfbSITh");

	std::string text = "test próba";
	std::string file = "test.wav";

	synth.convertTextToSpeech(text, file, SPEECH_POLISH);
}

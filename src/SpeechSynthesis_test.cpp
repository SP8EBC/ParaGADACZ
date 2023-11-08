/*
 * SpeechSynthesis_test.cpp
 *
 *	This tests checks only methods implemented at base class, universal
 *	across specific implementation
 *
 *  Created on: Sep 18, 2023
 *      Author: mateusz
 */

#define BOOST_TEST_MODULE SpeechSynthesis_test
#include <boost/test/included/unit_test.hpp>

#include "SpeechSynthesisResponsivevoice.h"
#include "EmailDownloader.h"
#include "TimeTools.h"
#include "PlaylistAssembler.h"

#include "secret.h"

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

ConfigurationFile_Email configEmail;
ConfigurationFile_Email_AllowedSender sender, sender2, sender3;

std::shared_ptr<PlaylistSampler> playlist_sampler;
std::shared_ptr<ConfigurationFile> configuration_file_first; //("./test_input/configuration_playlist_assembler_first.conf");

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/speechsynthesis_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

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

    configuration_file_first = std::make_shared<ConfigurationFile>("./test_input/configuration_forecast_email_speech_synthesis.conf");
    configuration_file_first->parse();


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
	SpeechSynthesisResponsivevoice synth("kvfbSITh", 0.5f, 0.5f, 30.0f);

	const std::string fn = "./test_input/ttsIndex1.json";

	int result;

	BOOST_CHECK_NO_THROW(result = synth.readIndex(fn));
	BOOST_CHECK_EQUAL(2, result);

	const std::list<SpeechSynthesis_MessageIndexElem>& index = synth.getIndexContent();
	std::list<SpeechSynthesis_MessageIndexElem>::const_iterator it = index.begin();

	BOOST_CHECK_EQUAL("test1.mp3", it->filename);
	BOOST_CHECK_EQUAL(12345, it->sayUntil);
	BOOST_CHECK_EQUAL("test@interia.pl", it->sender);
	BOOST_CHECK_EQUAL(12340, it->dispatchedAt);

	BOOST_CHECK_NO_THROW(it++);

	BOOST_CHECK_EQUAL("test2.mp3", it->filename);
	BOOST_CHECK_EQUAL(54321, it->sayUntil);
	BOOST_CHECK_EQUAL("test@gmail.pl", it->sender);
	BOOST_CHECK_EQUAL(54300, it->dispatchedAt);
}

BOOST_AUTO_TEST_CASE(validateSingleShot)
{

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

	SpeechSynthesisResponsivevoice synth(RESPONSIVEVOICE, 0.5f, 0.5f, 30.0f);

	const std::string fn = "./test_input/ttsIndex3.json";

	BOOST_CHECK_NO_THROW(synth.createIndex(fn));
	BOOST_CHECK_EQUAL(synth.getIndexContent().size(), 0);

	synth.convertEmailsToSpeech(messages, false, 0, SPEECH_POLISH, 10, 10, "");
	BOOST_CHECK_EQUAL(synth.getIndexContent().size(), 1);

	BOOST_CHECK_NO_THROW(synth.storeIndex());

	SpeechSynthesisResponsivevoice synthRead("xxxxx2", 0.5f, 0.5f, 30.0f);
	BOOST_CHECK_NO_THROW(synthRead.readIndex(fn));
	BOOST_CHECK_EQUAL(synthRead.getIndexContent().size(), 1);

	const std::list<SpeechSynthesis_MessageIndexElem>::const_iterator index = synthRead.getIndexContent().begin();
	const SpeechSynthesis_MessageIndexElem & firstElem = *index;

	// single shot message is saved in index with sayUntil value of 666 and receivedAt set
	// to original timestamp. When this index is read receivedAt is overwritten with zero
	// to indicate that this is single shot anouncement played before. When the index
	// is written one more time this value is stored as zero
	BOOST_CHECK_EQUAL(firstElem.sayUntil, 666ULL);
	BOOST_CHECK_EQUAL(firstElem.sender, sender.emailAddress);
	BOOST_CHECK_EQUAL(firstElem.dispatchedAt, 0);
	BOOST_CHECK_EQUAL(firstElem.filename, "202CB962AC59075B964B07152D234B70.mp3");


}

BOOST_AUTO_TEST_CASE(playlisttAssemblerSingleShot)
{
	std::string address2("noreply@bandcamp.com");
	std::string address(sender.emailAddress);
	std::string topic2 = "single";
	std::string emailDispatchDateTime = "Wed, 8 Nov 2023 13:56:31 +0000";
	uint64_t emailDispatchUtcTimestamp = 1699451791ULL;
	uint64_t emailReceiveUtcTimestmp = 1699451791ULL;
	std::string originalEncoding = "7bit";
	std::string originalCharset = "us-ascii";

	tm datetime;

	// copy date time into tm structure
	datetime.tm_year = 2023;
	datetime.tm_mon = 11;
	datetime.tm_mday = 8;
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

	msg1.setValidated();
	msg1.setValidUntil(EMAILDOWNLOADERMESSAGE_VALIDUNTIL_SINGLESHOT_ANNOUNCEMENT);

	std::vector<EmailDownloaderMessage> messages;
	messages.push_back(msg1);

	// used only to erase tts index, which also remove tts
	SpeechSynthesisResponsivevoice synth(RESPONSIVEVOICE, 0.5f, 0.5f, 30.0f);

	const ConfigurationFile_SpeechSynthesis speech_synthesis_config = configuration_file_first->getSpeechSynthesis();

	BOOST_CHECK_NO_THROW(synth.createIndex(speech_synthesis_config.indexFilePath));

	// assemble playlist with that announcement
	PlaylistAssembler assembler(playlist_sampler, configuration_file_first);

	PlaylistAssembler_TextToSpeechAnnouncement_Stats stats;
	BOOST_CHECK_NO_THROW(assembler.start());
	BOOST_CHECK_NO_THROW(stats = assembler.textToSpeechAnnouncements(messages));

	std::shared_ptr<std::vector<std::string>> playlist_ptr = assembler.getPlaylist();
	std::vector<std::string> playlist = *playlist_ptr;
	int i = 0;
	BOOST_CHECK_EQUAL(1, stats.added);
	BOOST_CHECK_EQUAL(1, stats.addedSingleShot);
	BOOST_CHECK_EQUAL(2, playlist_ptr->size());
	BOOST_CHECK_EQUAL("intro.ogg", playlist[i++]);
	BOOST_CHECK_EQUAL("202CB962AC59075B964B07152D234B70.mp3", playlist[i++]);

	// reassemble playlist one more time to check if single shot is not added
	BOOST_CHECK_NO_THROW(assembler.start());
	BOOST_CHECK_NO_THROW(stats = assembler.textToSpeechAnnouncements(messages));

	std::shared_ptr<std::vector<std::string>> playlist2_ptr = assembler.getPlaylist();
	std::vector<std::string> playlist2 = *playlist2_ptr;

	BOOST_CHECK_EQUAL(0, stats.added);
	BOOST_CHECK_EQUAL(0, stats.addedSingleShot);
}


BOOST_AUTO_TEST_CASE(speechSynthesisUseOnlyLastMessage)
{
	std::string address("sklep@8a.pl");
	std::string topic2 = "single";
	std::string emailDispatchDateTime = "Wed, 6 Sep 2023 13:56:31 +0000";
	std::string emailDispatchDateTime2 = "Wed, 6 Sep 2023 14:56:31 +0000";
	uint64_t emailDispatchUtcTimestamp = 1699451791ULL;
	uint64_t emailDispatchUtcTimestamp2 = 1699455391ULL;
	uint64_t emailReceiveUtcTimestmp = 1699533288ULL;
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

	datetime.tm_hour = 14;

	// convert that tm to have dispatch time&date in boost local_date_time format
	boost::local_time::local_date_time emailDispatchBoostDate2 =
			TimeTools::getLocalTimeFromTmStructAndTzOffset(datetime, true, GMT);

	EmailDownloaderMessage msg2(
							address,
							topic2,
							emailDispatchDateTime2,
							emailDispatchBoostDate2,
							0,
							emailDispatchUtcTimestamp2,
							emailReceiveUtcTimestmp,
							"123",
							"123",
							originalEncoding,
							originalCharset);

	std::vector<EmailDownloaderMessage> messages;
	msg1.setValidated();
	msg2.setValidated();
	messages.push_back(msg1);
	messages.push_back(msg2);

	SpeechSynthesisResponsivevoice synth(RESPONSIVEVOICE, 0.5f, 0.5f, 30.0f);

	synth.createIndex("./test_input/ttsIndex4_UseOnlyLastMessage.json");

	const SpeechSynthesis_ConvertEmailsToSpeech_Stats result =
			synth.convertEmailsToSpeech(messages, true, 1234, SPEECH_POLISH, 10, 10, "");

	BOOST_CHECK_EQUAL(emailDispatchUtcTimestamp2, result.latestDispatchTimestamp);
	BOOST_CHECK_EQUAL(1, result.notNewestSetAsOutdated);
	BOOST_CHECK_EQUAL(1, result.converted);



}


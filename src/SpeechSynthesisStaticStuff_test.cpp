/*
 * SpeechSynthesisStaticStuff_test.cpp
 *
 *  Created on: Sep 14, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesisStaticStuff.h"

#define BOOST_TEST_MODULE SpeechSynthesisStaticStuff_test
#include <boost/test/included/unit_test.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

ConfigurationFile_Email configEmail;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/speechsynthesisstaticstuff_test.log" )
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

BOOST_AUTO_TEST_CASE(cutParagraphsFromText_1)
{
	ConfigurationFile_Email_AllowedSender_Preprocess config = {0u};

	std::string testinput = "dsfdsfsd\r\n"
			"sdfdfdsfdf\r\n\r\n\r\n"
			"sdfdfsdfsdfd\r\n";

	std::string expected = "dsfdsfsd\r\n"
			"sdfdfdsfdf\r\n"
			"sdfdfsdfsdfd\r\n";

	std::string result = SpeechSynthesisStaticStuff::cutParagraphsFromText(config, testinput);

	BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(cutParagraphsFromText_2)
{
	ConfigurationFile_Email_AllowedSender_Preprocess config = {0u};

	std::string testinput = "dsfdsfsd\r"
			"sdfdfdsfdf\r\r\r\r"
			"sdfdfsdfsdfd\r";

	std::string expected = "dsfdsfsd\r\n"
			"sdfdfdsfdf\r\n"
			"sdfdfsdfsdfd\r\n";

	std::string result = SpeechSynthesisStaticStuff::cutParagraphsFromText(config, testinput);

	BOOST_CHECK_EQUAL(result, expected);

}

BOOST_AUTO_TEST_CASE(cutParagraphsFromText_2_limitlower)
{
	ConfigurationFile_Email_AllowedSender_Preprocess config = {0u};
	config.startFromParagraph = 1;

	std::string testinput = "dsfdsfsd\r"
			"sdfdfdsfdf\r\r\r\r"
			"sdfdfsdfsdfd\r";

	std::string expected =
			"sdfdfdsfdf\r\n"
			"sdfdfsdfsdfd\r\n";

	std::string result = SpeechSynthesisStaticStuff::cutParagraphsFromText(config, testinput);

	BOOST_CHECK_EQUAL(result, expected);

}

BOOST_AUTO_TEST_CASE(cutParagraphsFromText_2_limitupper)
{
	ConfigurationFile_Email_AllowedSender_Preprocess config = {0u};
	config.endOnParagraph = 1;

	std::string testinput = "dsfdsfsd\r"
			"sdfdfdsfdf\r\r\r\r"
			"sdfdfsdfsdfd\rrrrtttt444545";

	std::string expected = "dsfdsfsd\r\n";

	std::string result = SpeechSynthesisStaticStuff::cutParagraphsFromText(config, testinput);

	BOOST_CHECK_EQUAL(result, expected);

}

BOOST_AUTO_TEST_CASE(cutParagraphsFromText_2_limitboth)
{
	ConfigurationFile_Email_AllowedSender_Preprocess config = {0u};
	config.startFromParagraph = 2;
	config.endOnParagraph = 5;

	std::string testinput = "dsfdsfsd\r"
			"sdfdfdsfdf\r\r\r\r"
			"sdfdfsdfsdfd\rrrrtttt444545\r\n\r\nzxcvbnm\r\n111";

	std::string expected = "sdfdfsdfsdfd\r\nrrrtttt444545\r\nzxcvbnm\r\n";

	std::string result = SpeechSynthesisStaticStuff::cutParagraphsFromText(config, testinput);

	BOOST_CHECK_EQUAL(result, expected);

}

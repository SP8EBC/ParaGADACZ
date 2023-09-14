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

	SpeechSynthesisStaticStuff::cutParagraphsFromText(config, testinput);
}

BOOST_AUTO_TEST_CASE(cutParagraphsFromText_2)
{
	ConfigurationFile_Email_AllowedSender_Preprocess config = {0u};

	std::string testinput = "dsfdsfsd\r"
			"sdfdfdsfdf\r\r\r\r"
			"sdfdfsdfsdfd\r";

	SpeechSynthesisStaticStuff::cutParagraphsFromText(config, testinput);
}


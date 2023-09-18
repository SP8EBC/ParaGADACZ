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

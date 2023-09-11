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

BOOST_AUTO_TEST_CASE(second)
{
	SpeechSynthesisResponsivevoice synth("kvfbSITh");

	std::string text = "test próba";
	std::string file = "test.wav";

	synth.convertTextToSpeech(text, file, SPEECH_POLISH);
}

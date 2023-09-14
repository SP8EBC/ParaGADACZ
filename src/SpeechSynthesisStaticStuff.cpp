/*
 * SpeechSynthesisStaticStuff.cpp
 *
 *  Created on: Sep 14, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesisStaticStuff.h"

#include <boost/algorithm/string.hpp>

SpeechSynthesisStaticStuff::SpeechSynthesisStaticStuff() {
	// TODO Auto-generated constructor stub

}

std::string SpeechSynthesisStaticStuff::cutParagraphsFromText(
		ConfigurationFile_Email_AllowedSender_Preprocess &config,
		std::string &text) {

	std::vector<std::string> separated;

	boost::split(separated, text, boost::is_any_of("\r\n"));

	return std::string();

}

SpeechSynthesisStaticStuff::~SpeechSynthesisStaticStuff() {
	// TODO Auto-generated destructor stub
}


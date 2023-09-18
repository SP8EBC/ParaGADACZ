/*
 * SpeechSynthesisStaticStuff.cpp
 *
 *  Created on: Sep 14, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesisStaticStuff.h"

#include <sstream>

#include <boost/algorithm/string.hpp>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

SpeechSynthesisStaticStuff::SpeechSynthesisStaticStuff() {
	// TODO Auto-generated constructor stub

}

std::string SpeechSynthesisStaticStuff::cutParagraphsFromText(
		const ConfigurationFile_Email_AllowedSender_Preprocess &config,
		const std::string &text) {

	std::vector<std::string> separated;

	std::stringstream output;

	// paragraphs counter
	int paragraphs = 0;

	int missedParagraph = 0;

	// split input by newline
	boost::split(separated, text, boost::is_any_of("\r\n"));

	// go through splitting output
	for (std::string & str : separated) {
		// check minimum length
		if (str.length() < 3) {
			missedParagraph++;

			// this is too short, maybe empty string from multiple enters
			continue;
		}

		// check if announcement should start from n-paragraph or from begining
		if (config.startFromParagraph != 0 || config.endOnParagraph != 0) {
			// check if this is the paragraph we should include
			if (paragraphs >= config.startFromParagraph && (paragraphs < config.endOnParagraph || (config.endOnParagraph == 0))) {
				// append to output
				output << str << "\r\n";
			}
		}
		else {
			// everything should be included, just tub out
			output << str << "\r\n";
		}

		// increment paragraph counter
		paragraphs++;
	}

	SPDLOG_DEBUG("{} paragraphs have been added to an output, {} elements was skipped", paragraphs, missedParagraph);

	return output.str();

}

SpeechSynthesisStaticStuff::~SpeechSynthesisStaticStuff() {
	// TODO Auto-generated destructor stub
}


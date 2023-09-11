/*
 * SpeechSynthesis.h
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */

#ifndef SPEECHSYNTHESIS_H_
#define SPEECHSYNTHESIS_H_

#include <string>

enum SpeechSynthesis_Language {
	SPEECH_POLISH,
	SPEECH_ENGLISH
};

class SpeechSynthesis {
public:

	virtual void convertTextToSpeech(std::string & text, std::string & outputFilename, SpeechSynthesis_Language lang) = 0;

	SpeechSynthesis();
	virtual ~SpeechSynthesis();
};

#endif /* SPEECHSYNTHESIS_H_ */

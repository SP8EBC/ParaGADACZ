/*
 * SpeechSynthesis.h
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */

#ifndef SPEECHSYNTHESIS_H_
#define SPEECHSYNTHESIS_H_

#include <string>

struct SpeechSynthesis_MessageIndexElem {
	std::string filename;	//!< Filename with an synthesized anouncement
	uint64_t sayUntil;		//!< A timestamp after which this announcement will not be said
	std::string sender;		//!< Sender of this message
	uint64_t reveivedAt;	//!< Time when this announcement was received
};

enum SpeechSynthesis_Language {
	SPEECH_POLISH,
	SPEECH_ENGLISH
};

class SpeechSynthesis {
public:

	bool readIndex(std::string & indexFn);

	virtual void convertTextToSpeech(std::string & text, std::string & outputFilename, SpeechSynthesis_Language lang) = 0;

	SpeechSynthesis();
	virtual ~SpeechSynthesis();
};

#endif /* SPEECHSYNTHESIS_H_ */

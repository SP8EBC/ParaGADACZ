/*
 * SpeechSynthesisStaticStuff.h
 *
 *  Created on: Sep 14, 2023
 *      Author: mateusz
 */

#ifndef SPEECHSYNTHESISSTATICSTUFF_H_
#define SPEECHSYNTHESISSTATICSTUFF_H_

#include "ConfigurationFile.h"

/**
 * All auxiliary functionality required by speech synthesis, mostly text
 * processing
 */
class SpeechSynthesisStaticStuff {
public:

	static std::string cutParagraphsFromText(const ConfigurationFile_Email_AllowedSender_Preprocess & config, const std::string & text);

	SpeechSynthesisStaticStuff();
	virtual ~SpeechSynthesisStaticStuff();
};

#endif /* SPEECHSYNTHESISSTATICSTUFF_H_ */

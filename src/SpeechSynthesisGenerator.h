/*
 * SpeechSynthesisGenerator.h
 *
 *  Created on: Sep 4, 2023
 *      Author: mateusz
 */

#ifndef SPEECHSYNTHESISGENERATOR_H_
#define SPEECHSYNTHESISGENERATOR_H_

/**
 * This class wraps around speech synthesis service, takes an input in form of
 * an email and then converts that into audio file to be played by the program.
 * Generated audio files are indexed and stored into
 *
 */
class SpeechSynthesisGenerator {
public:
	SpeechSynthesisGenerator();
	virtual ~SpeechSynthesisGenerator();
};

#endif /* SPEECHSYNTHESISGENERATOR_H_ */

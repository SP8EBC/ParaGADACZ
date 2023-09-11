/*
 * SpeechSynthesisResponsivevoice.h
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */

#ifndef SPEECHSYNTHESISRESPONSIVEVOICE_H_
#define SPEECHSYNTHESISRESPONSIVEVOICE_H_

#include "SpeechSynthesis.h"
#include <cstdint>
#include <vector>

class SpeechSynthesisResponsivevoice: public SpeechSynthesis {

	/**
	 * Api key
	 */
	std::string key;

	static size_t staticWriteCallback(char* get_data, size_t always_one, size_t get_data_size, void* userdata);

	/**
	 * Write callback uset to store HTTP server response
	 */
	void writeCallback(char* data, size_t data_size);

	/**
	 *
	 */
	std::vector<uint8_t> audiofile;

public:
	SpeechSynthesisResponsivevoice(std::string apiKey);
	virtual ~SpeechSynthesisResponsivevoice();
	virtual void convertTextToSpeech(std::string &text,
			std::string &outputFilename, SpeechSynthesis_Language lang);
};

#endif /* SPEECHSYNTHESISRESPONSIVEVOICE_H_ */

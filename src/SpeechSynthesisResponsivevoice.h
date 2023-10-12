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

	std::string pitch;

	std::string rate;

	/**
	 * curl maximum timeout
	 */
	float timeout;

	/**
	 * Write callback uset to store HTTP server response
	 */
	void writeCallback(char* data, size_t data_size);

	/**
	 * Vector where binary data from a response will be stored.
	 */
	std::vector<uint8_t> audiofile;

	SpeechSynthesisResponsivevoice * ptr;

	/**
	 * Used for debug purposes to store text being converted to speech
	 * @param text
	 * @param filename
	 */
	void storeTextInFile(std::string & text, std::string & filename);

	static size_t staticWriteCallback(char* get_data, size_t always_one, size_t get_data_size, void* userdata);

public:
	SpeechSynthesisResponsivevoice(std::string apiKey, float pitch, float rate, float httpTimeout);
	virtual ~SpeechSynthesisResponsivevoice();
	virtual void convertTextToSpeech(std::string &text,
			std::string &outputFilename, ConfigurationFile_Language lang);
};

#endif /* SPEECHSYNTHESISRESPONSIVEVOICE_H_ */

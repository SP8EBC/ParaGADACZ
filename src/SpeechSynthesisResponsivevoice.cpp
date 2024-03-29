/*
 * SpeechSynthesisResponsivevoice.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesisResponsivevoice.h"
#include "main.h"

#include <sstream>
#include <fstream>
#include <array>
#include <iterator>
#include <exception>

#include "curl/curl.h"
#include "curl_code_to_str.h"

#include "exception/SpeechSynthesisTimeoutEx.h"
#include "exception/SpeechSynthesisHttpErrorCodeEx.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/bin_to_hex.h"
#pragma pop_macro("U")

SpeechSynthesisResponsivevoice::SpeechSynthesisResponsivevoice(std::string apiKey, float pitch, float rate, float httpTimeout) : key(apiKey) {
	ptr = this;

	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << pitch;
	this->pitch = stream.str();

	stream.str("");
	stream << std::fixed << std::setprecision(1) << rate;
	this->rate = stream.str();

	timeout = httpTimeout;

}

SpeechSynthesisResponsivevoice::~SpeechSynthesisResponsivevoice() {
	// TODO Auto-generated destructor stub
}

size_t SpeechSynthesisResponsivevoice::staticWriteCallback(char *get_data,
		size_t always_one, size_t get_data_size, void *userdata) {

	SpeechSynthesisResponsivevoice* class_ptr = static_cast<SpeechSynthesisResponsivevoice*>(userdata);

	class_ptr->writeCallback(get_data, get_data_size * always_one);

	return get_data_size * always_one;
}

void SpeechSynthesisResponsivevoice::writeCallback(char *data,
		size_t data_size) {

	const uint8_t * ptr = reinterpret_cast<uint8_t*> (data);

	for (size_t i = 0; i < data_size; i++) {
		this->audiofile.push_back(ptr[i]);
	}

}

void SpeechSynthesisResponsivevoice::storeTextInFile(std::string &text,
		std::string &filename) {

    std::fstream file(filename + ".txt", std::ios_base::out);

    if (file.is_open()) {
    	file << text;

    	file.sync();

    	file.close();
    }

}

void SpeechSynthesisResponsivevoice::convertTextToSpeech(std::string &text,
		std::string &outputFilename, ConfigurationFile_Language lang) {

	CURLcode result = CURLcode::CURLE_OBSOLETE50;

	// string stream to assemble URL into
	std::stringstream urlStream;

    char* effectiveUrl;
    long response_code;

	// initialize cURL
    auto curl = curl_easy_init();

    // clear vector where a response with an audio file will be stored
    this->audiofile.clear();

#ifdef MAIN_STORE_TEXT_FOR_SPEECH_SYNTHESIS
    storeTextInFile(text, outputFilename);
#endif

    // assembly URL
    urlStream << "https://texttospeech.responsivevoice.org/v1/text:synthesize?lang=";

    // select which language should be used
    switch(lang) {
    case SPEECH_POLISH:
    	urlStream << "pl";
    	break;
    case SPEECH_ENGLISH:
    	urlStream << "en";
    	break;
    }

    // append constant part
    urlStream << "&engine=g1&name=&pitch=" << pitch << "&rate=" << rate << "&volume=1&key=";

    // append api key
    urlStream << key;

    // append next constant secion of the URL
    urlStream << "&gender=female&text=";

    // encode text
    const char * encodedText = curl_easy_escape(curl, text.c_str(), text.size());

    // construct a string with encoded string
    const std::string encodedTextStr(encodedText);

    // release curl resources
    curl_free((void*)encodedText);

    urlStream << encodedTextStr;

    curl_easy_setopt(curl, CURLOPT_URL, urlStream.str().c_str());		// set URL to query
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);			// shutdown progress meter and do not call progress function cbk
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");	// UserAgent identification
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);			// how many HTTP redirects are allowed before cURL will gave up
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (int)this->timeout);	// maximum HTTP transaction timeout in seconds
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, (int)(this->timeout * 1000.0f));				// the same but in miliseconds ???? why doubled ??
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &SpeechSynthesisResponsivevoice::staticWriteCallback);	// needs to be bind
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)ptr);

    // send HTTP request and wait for response
    result = curl_easy_perform(curl);

    // get all results
    double elapsed;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveUrl);

    SPDLOG_DEBUG("CURL response_code: {}", boost::lexical_cast<std::string>(response_code));
    SPDLOG_DEBUG("CURL query elapsed time: {}", boost::lexical_cast<std::string>(elapsed));
    SPDLOG_DEBUG("CURL result: {}", curlCodeToStr(result));

    // deinitialize cURL
    curl_easy_cleanup(curl);
    curl = NULL;

    if (result != CURLE_OPERATION_TIMEDOUT) {
		if (response_code == 200) {
			// store data into a file
			std::fstream file(outputFilename, std::ios_base::binary | std::ios_base::out);

			if (file.is_open()) {
				SPDLOG_INFO("Storing TTS audio file in {}", outputFilename);

				std::copy(audiofile.begin(), audiofile.end(), std::ostream_iterator<uint8_t>(file));

				file.sync();

				file.close();
			}
			else {
				SPDLOG_ERROR("Something happend while output file was opening");
			}

		}
		else {
			SPDLOG_ERROR("Error code {} received from API during TTS conversion for {}", boost::lexical_cast<std::string>(response_code), outputFilename);
			throw SpeechSynthesisHttpErrorCodeEx();
		}
    }
    else {
		SPDLOG_ERROR("Communication with the API timed out during TTS conversion for {}", outputFilename);
		throw SpeechSynthesisTimeoutEx();
    }

}

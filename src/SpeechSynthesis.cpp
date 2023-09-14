/*
 * SpeechSynthesis.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesis.h"
#include "TimeTools.h"

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/algorithm/hex.hpp"

#include <nlohmann/json.hpp>	// JSON library

 #include <openssl/md5.h>

#include <fstream>
#include <exception>
#include <iterator>

constexpr SpeechSynthesis_FindValidAnouncements_UnaryPredicate SpeechSynthesis_getValidAnouncements_pred;

SpeechSynthesis::SpeechSynthesis() {
	// TODO Auto-generated constructor stub

}

SpeechSynthesis::~SpeechSynthesis() {
	// TODO Auto-generated destructor stub
}

/**
 * Opens and reads an index of announcements index
 * @param indexFn path + name of index JSON file
 * @return true if file was opened or false if doesn't exist
 * @throws
 */
bool SpeechSynthesis::readIndex(std::string &indexFn) {

	// set to true if index has been read
	bool out = false;

	// input buffer for JSON data read from file
	std::string inputBuffer;

	// path to cache index in temporary directory
	boost::filesystem::path index(indexFn);

	// check if index file exists
	if (boost::filesystem::exists(index)) {

		// check if this is a regular file or not
		if (boost::filesystem::is_regular_file(index)) {

			// open file from disk
			std::fstream inputFile;
			inputFile.open(index.generic_string(), std::ios::in);

			// check if input file has been opened
			if (inputFile.is_open()) {

				SPDLOG_INFO("Loading speech synthesis index file {}", index.generic_string());

				try {
					// reserve place for whole index file
					inputFile.seekg(0, std::ios::end);
					inputBuffer.resize(inputFile.tellg());
				}
				catch (std::length_error & er) {
					SPDLOG_ERROR("std::length_error has been thrown while resizing string buffer for loading");
					SPDLOG_ERROR(er.what());
				}

				// and rewind back to the begining
				inputFile.seekg(0);

				// read all content
				inputFile.read(inputBuffer.data(), inputBuffer.size());

				// close input file as it is not needed anymore
				inputFile.close();

				// try to parse JSON read from file
				nlohmann::basic_json json = nlohmann::json::parse(inputBuffer, nullptr, false);

				// check if parsing was successfull
				if (nlohmann::json::value_t::discarded != json) {

					// store a filename
					indexFilename = indexFn;

					// index is a root label for the content
					if (json.contains("announcements")) {
						nlohmann::json array = json["announcements"];

						// get number of elements
						nlohmann::json::size_type elemNumbers = array.size();

						for (unsigned i = 0 ; i < static_cast<unsigned>(elemNumbers); i++) {

							// get element from index
							nlohmann::json elem = array[i];

							// each element shall consist exactly four elements
							if (elem.size() == 4) {
								// get all values from JSON
								std::string _filename = elem["filename"];
								uint64_t _sayUntil = elem["sayUntil"];
								std::string _sender = elem["sender"];
								uint64_t _receivedAt = elem["receivedAt"];

								// create index element
								SpeechSynthesis_MessageIndexElem indexElement;

								indexElement.filename = _filename;
								indexElement.receivedAt = _receivedAt;
								indexElement.sayUntil = _sayUntil;
								indexElement.sender = _sender;

								// add new element to index vector
								this->indexContent.push_back(indexElement);
							}
						}

						//
						out = true;
					}
					else {
						SPDLOG_ERROR("Speech synthesis index file has unknown structure");
					}
				}
				else {
					SPDLOG_ERROR("Speech synthesis index file contains a garbage which cannot be parsed by JSON library");
					throw std::runtime_error("");
				}
			}
			else {
				SPDLOG_ERROR("Speech synthesis index file exists but cannot be opened for some reason");
				throw std::runtime_error("");
			}
		}
		else {
			SPDLOG_ERROR("Path provided as an index file doesn't point to any valid file, maybe it is a directory os something else");
			throw std::runtime_error("");
		}
	}
	else {
		// if not new one will be created
		SPDLOG_INFO("Speech synthesis index file hasn't been created yet.");
	}

	return out;
}

/**
 * Writes index back into a JSON file.
 * @param indexFn file where index will be written into, any existing content will be overwritten.
 */
void SpeechSynthesis::storeIndex() {
}

/**
 * Appends all announcements audio files which are valid (not too old) to be played now.
 * These paths are read from index file, so before a call to this file all new emails
 * must be converted from text to speech.
 * @param playlist
 * @return how many new elements were added
 */
int SpeechSynthesis::getValidAnouncements(std::vector<std::string> &playlist) {

	int output = 0;

	// get current time
	uint64_t currentTime = TimeTools::getEpoch();

	bool rewriteIndex = false;

	// get through an index
	for (std::list<SpeechSynthesis_MessageIndexElem>::const_iterator it = indexContent.begin(); it != indexContent.end(); it++) {

		// check if this message is not too old
		if (it->sayUntil > currentTime ) {
			// append audio file into playlist
			playlist.push_back(it->filename);
		}
		else {
			// remove from index if it is too old
			indexContent.erase(it);

			// set a flag that JSON file with an index should be updated
			rewriteIndex = true;
		}
	}

	// if any element was removed because it was overdued
	if (rewriteIndex) {
		// rewrite
		storeIndex();
	}

	return output;
}

/**
 * Takes a vector with emails taken from inbox, checks if they have been already
 * converted to speech and does that conversion if it is neccessary.
 * @param msgs vector of emails from an inbox
 * @param ignoreOlderThan if set to non zero all emails older than this amount of minutes will
 * 							be automatically ignored without checking if they are already converted
 * 							to speech and if they require such conversion
 */
void SpeechSynthesis::convertEmailsToSpeech(
		std::vector<EmailDownloaderMessage> &msgs, const uint32_t ignoreOlderThan) {

	const uint64_t currentTime = TimeTools::getEpoch();

	// context used for MD5
	MD5_CTX md5_context;

	// initialize md5 context
	MD5_Init(&md5_context);

	std::array<uint8_t, MD5_DIGEST_LENGTH> md5hashBinary;

	SPDLOG_INFO("Converting {} emails from text to speech", msgs.size());

	// go through all messages in vector
	for (EmailDownloaderMessage msg : msgs) {

		// index element to store information about currently processed email
		SpeechSynthesis_MessageIndexElem indexElem;

		// stream to store hash valie in
		std::stringstream md5hash;

		// output stream iterator to
		std::ostream_iterator<char> md5hashIt(md5hash);

		// if old messages should be skipped
		if (ignoreOlderThan > 0) {
			// if this message has been sent before given date
			if (msg.getEmailDispatchUtcTimestamp() + (const uint64_t)ignoreOlderThan * 60ULL > currentTime) {
				// continue to next one
				continue;
			}
		}

		const std::string & emailContent = msg.getContent();

		// calculate md5 hash of email content
		MD5_Update(&md5_context, emailContent.data(), emailContent.size());

		// finalize MD5 calculation
		MD5_Final(md5hashBinary.data(), &md5_context);

		boost::algorithm::hex(md5hashBinary.begin(), md5hashBinary.end(), md5hashIt);

		indexElem.filename = md5hash.str();

		this->indexContent.push_back(indexElem);

	}
}



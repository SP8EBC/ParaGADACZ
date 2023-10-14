/*
 * SpeechSynthesis.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesis.h"
#include "SpeechSynthesisStaticStuff.h"
#include "TimeTools.h"

#include "exception/SpeechSynthesisTooMuchErrorsEx.h"

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

#define SPEECHSYNTHESIS_ERROR_REPETITIONS_LIMIT	5

SpeechSynthesis::SpeechSynthesis() {
	// TODO Auto-generated constructor stub

}

SpeechSynthesis::~SpeechSynthesis() {
	// TODO Auto-generated destructor stub
}

void SpeechSynthesis::createIndex(const std::string &indexFn) {
	indexFilename = indexFn;

	// root of a document
	nlohmann::json json = nlohmann::json::object();

	// array with index elements
	nlohmann::json arr = nlohmann::json::array();

	// create single list object
	nlohmann::basic_json _indexElementJson = nlohmann::json::object();

	// fill it with data
	_indexElementJson["filename"] = "dummy";
	_indexElementJson["sayUntil"] = 0;
	_indexElementJson["sender"] = "dummy";
	_indexElementJson["receivedAt"] = 0;

	// and add to the array
	arr.push_back(_indexElementJson);

	json["announcements"] = arr;

	// path to cache index in temporary directory
	boost::filesystem::path index(indexFilename);

	// remove index file if it has been creaeted
	const bool existed = boost::filesystem::remove(index);

	std::fstream indexStream;

	// open a file with an index and truncate it's content
	indexStream.open(index.generic_string(), std::ios::out | std::ios::trunc);

	// put JSON into file on disk
	indexStream << json;

	indexStream.close();

	indexContent = std::list<SpeechSynthesis_MessageIndexElem>();
}

/**
 * Opens and reads an index of announcements index
 * @param indexFn path + name of index JSON file
 * @return true if file was opened or false if doesn't exist
 * @throws
 */
int SpeechSynthesis::readIndex(const std::string &indexFn) {

	// how many entries has been read from index
	int out = -1;

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

						// clear existing index
						indexContent.clear();

						out = 0;

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

								// check if this is dummy entry
								if (_sayUntil == 0 && _receivedAt == 0) {
									continue;	// ignore it
								}

								// check if it was single shot announcement played in the past
								if (_sayUntil == EMAILDOWNLOADERMESSAGE_VALIDUNTIL_SINGLESHOT_ANNOUNCEMENT) {
									// if yes overwrite receivedAt timestamp
									// to indicate that this is a single shot
									// message read before.
									// PlaylistSampler works in the way that it
									// ignores everything with _receivedAt set to 0
									_receivedAt = 0;
								}

								// create index element
								SpeechSynthesis_MessageIndexElem indexElement;

								indexElement.filename = _filename;
								indexElement.receivedAt = _receivedAt;
								indexElement.sayUntil = _sayUntil;
								indexElement.sender = _sender;

								// add new element to index vector
								this->indexContent.push_back(indexElement);

								out++;
							}
						}
					}
					else {
						SPDLOG_ERROR("Speech synthesis index file has unknown structure");
						throw std::runtime_error("");
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
			throw std::invalid_argument("");
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
	if (indexFilename.length() < 3) {
		throw std::runtime_error("Index file hasn't been opened yet");
	}

	if (indexContent.size() == 0) {
		throw std::runtime_error("There is nothing to save an index for");
	}

	// path to cache index in temporary directory
	boost::filesystem::path index(this->indexFilename);

	std::fstream indexStream;

	// open a file with an index and truncate it's content
	indexStream.open(index.generic_string(), std::ios::out | std::ios::trunc);

	// check if index file has been opened for writing
	if (indexStream.is_open()) {
		// root of a document
		nlohmann::json json = nlohmann::json::object();

		// array with index elements
		nlohmann::json arr = nlohmann::json::array();

		SPDLOG_INFO("Writing index into a file {}", index.generic_string());

		for (const auto & elem : indexContent) {

			// check if this is single shot anouncement
			if (elem.sayUntil == EMAILDOWNLOADERMESSAGE_VALIDUNTIL_SINGLESHOT_ANNOUNCEMENT) {
				SPDLOG_DEBUG("This is single-shot anouncement which will be sayd only once.");
			}

			if (elem.filename == "to_be_removed") {
				continue;
			}

			// create single list object
			nlohmann::basic_json _indexElementJson = nlohmann::json::object();

			// fill it with data
			_indexElementJson["filename"] = (elem.filename);
			_indexElementJson["sayUntil"] = (elem.sayUntil);
			_indexElementJson["sender"] = (elem.sender);
			_indexElementJson["receivedAt"] = (elem.receivedAt);

			// and add to the array
			arr.push_back(_indexElementJson);
		}

		// insert array into JSON structure
		json["announcements"] = arr;

		// put JSON into file on disk
		indexStream << json;

		indexStream.close();
	}
	else {

	}
}

/**
 * This is a callback used by Playlist Assembler on every message which is
 * used on all outdated messages which should not be sayid and than removed
 * from an index and probably even for a filesystem. The thing is that
 * it must be used only for messages which are older than 'ignoreOlderThan'
 * parameter in Text-to-seech configuration. Otherwise an email will be parsed
 * and converted once again during next program startup, even that it won't
 * be said at all. Playlist Assembler will detest it as an outdated one
 * and call this once again etc.
 *
 * @param elem element to be removed
 */
void SpeechSynthesis::removeOldMessage(SpeechSynthesis_MessageIndexElem & elem, bool saveIndexAuto) {

	// only alter the element name with this special keyword, recognized
	// by index saving routine. Element cannot be removed from a list
	// directly, because this invalidates interators and may cause a crash
	// if this method is called from inside a loop iterating through a list.
	elem.filename = "to_be_removed";

	if (saveIndexAuto) {
		this->storeIndex();
	}
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
 * @param lang
 * @param maximumTries
 * @param delayAfterFailedTry
 * @param audioFilesBaseDir
 */
void SpeechSynthesis::convertEmailsToSpeech(
										const std::vector<EmailDownloaderMessage> & msgs,
										uint32_t ignoreOlderThan,
										const ConfigurationFile_Language lang,
										int maximumTries,
										uint8_t delayAfterFailedTry,
										const std::string & audioFilesBaseDir) {

	const uint64_t currentTime = TimeTools::getEpoch();

	int notValid = 0;
	int tooOld = 0;
	int converted = 0;

	// counter of repetitions of
	int repetitions = 0;

	// context used for MD5
	MD5_CTX md5_context;

	// initialize md5 context
	MD5_Init(&md5_context);

	std::array<uint8_t, MD5_DIGEST_LENGTH> md5hashBinary;

	// go through all messages in vector
	for (EmailDownloaderMessage msg : msgs) {

		// skip any message which is not validated
		if (!msg.isValidated()) {
			notValid++;

			continue;
		}

		// if old messages should be skipped
		if (ignoreOlderThan > 0) {

			const uint64_t emailDispatchTime = msg.getEmailDispatchUtcTimestamp();

			// if this message has been sent before given date
			if (emailDispatchTime + (const uint64_t)ignoreOlderThan * 60ULL < currentTime) {

				SPDLOG_DEBUG("Message from {} send at {} UTC is too old", msg.getEmailAddress(), boost::posix_time::to_simple_string(TimeTools::getPtimeFromEpoch(msg.getEmailDispatchUtcTimestamp())));

				tooOld++;
				// continue to next one
				continue;
			}
		}

		// index element to store information about currently processed email
		SpeechSynthesis_MessageIndexElem indexElem;

		// stream to store hash valie in
		std::stringstream md5hash;

		if (audioFilesBaseDir.length() > 3) {
			md5hash << audioFilesBaseDir;
		}

		// output stream iterator to
		std::ostream_iterator<char> md5hashIt(md5hash);

		const std::string & emailContent = msg.getContent();

		// calculate md5 hash of email content
		MD5_Update(&md5_context, emailContent.data(), emailContent.size());

		// finalize MD5 calculation
		MD5_Final(md5hashBinary.data(), &md5_context);

		// convert MD5 into hex string
		boost::algorithm::hex(md5hashBinary.begin(), md5hashBinary.end(), md5hashIt);

		// append file extension
		md5hash << ".mp3";

		// store this hash
		indexElem.filename = md5hash.str();

		// look for this hash on the index
		std::list<SpeechSynthesis_MessageIndexElem>::const_iterator it =  std::find_if(indexContent.begin(), indexContent.end(), [&indexElem](SpeechSynthesis_MessageIndexElem & in) {
			if (indexElem.filename == in.filename) {
				return true;
			}
			else {
				return false;
			}
		});

		// this message has been already converted
		if (it != indexContent.end()) {
			// no need for further action
			SPDLOG_INFO("Message from {} received at {} UTC has been converted tts before. No action needed", it->sender, boost::posix_time::to_simple_string(TimeTools::getPtimeFromEpoch(it->receivedAt)) );
			continue;
		}

		// apply all text processing which is configured for that sender
		std::string text = SpeechSynthesisStaticStuff::cutParagraphsFromText(msg.getPreprocess(), msg.getContent());

		while(repetitions < maximumTries) {
			try {
				SPDLOG_INFO("Converting to speech message sent by {} at {} UTC", msg.getEmailAddress(), boost::posix_time::to_simple_string(TimeTools::getPtimeFromEpoch(msg.getEmailDispatchUtcTimestamp())));

				// convert text to speech
				this->convertTextToSpeech(text, indexElem.filename, lang);

				indexElem.receivedAt = msg.getEmailReceiveUtcTimestmp();
				indexElem.sayUntil = msg.getValidUntil();
				indexElem.sender = msg.getEmailAddress();

				this->indexContent.push_back(indexElem);

				converted++;

				break;
			}
			catch (std::exception & ex) {
				SPDLOG_ERROR("Exception was thrown while converting text to speech.");
				SPDLOG_ERROR(ex.what());
				repetitions++;

				if (delayAfterFailedTry > 0) {
					SPDLOG_INFO("Sleeping for {} seconds before next try");
					sleep(delayAfterFailedTry);
				}

				if (repetitions >= maximumTries) {
					throw SpeechSynthesisTooMuchErrorsEx();
				}
			}
		}

	}

	if (converted > 0) {
		// all messages converted so now store the index back on disk
		storeIndex();
	}

	SPDLOG_INFO("{} messsages converted, {} not valid, {} too old", converted, notValid, tooOld);
}



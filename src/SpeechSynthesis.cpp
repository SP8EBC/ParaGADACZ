/*
 * SpeechSynthesis.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: mateusz
 */

#include "SpeechSynthesis.h"

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp

SpeechSynthesis::SpeechSynthesis() {
	// TODO Auto-generated constructor stub

}

bool SpeechSynthesis::readIndex(std::string &indexFn) {

	// set to true if index has been read or created (if it didn't exist)
	bool out = false;
'
	// path to cache index in temporary directory
	boost::filesystem::path index(indexFn);

	return out;
}

SpeechSynthesis::~SpeechSynthesis() {
	// TODO Auto-generated destructor stub
}


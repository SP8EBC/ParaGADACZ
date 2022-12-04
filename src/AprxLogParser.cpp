/*
 * AprxLogParser.cpp
 *
 *  Created on: Dec 3, 2022
 *      Author: mateusz
 */

#include "AprxLogParser.h"

#include <sstream>

#define RTD_OFFSET			34
#define CALLSIGN_OFFSET		36
#define MINIMAL_LINE_LN		45

AprxLogParser::AprxLogParser(std::string fn) : fileName(fn) {
	parsedLines = 0;
}

AprxLogParser::~AprxLogParser() {
	// TODO Auto-generated destructor stub
}

AprxLogParser::AprxLogParser(const AprxLogParser &other) {
	// TODO Auto-generated constructor stub

}

AprxLogParser::AprxLogParser(AprxLogParser &&other) {
	// TODO Auto-generated constructor stub

}

AprxLogParser& AprxLogParser::operator=(const AprxLogParser &other) {
	// TODO Auto-generated method stub

}

AprsWXData AprxLogParser::getLastPacketForStation(std::string call,
		uint8_t ssid) {

	AprsWXData out;


    return out;

}

std::optional<std::string> AprxLogParser::getNextLine(std::string call,
		uint8_t ssid) {

	if (file.is_open()) {

		// local buffer for string line
		std::string buffer;

		// local buffer to extract callsign
		std::string bufCallsign;

		//
		char rtd = ' ';

		// read line by line
		while (std::getline(file, buffer)) {

			// check line length
			if (buffer.length() > MINIMAL_LINE_LN) {

				// get R/T/d indicator
				rtd = buffer.at(RTD_OFFSET);

				// continue to next line if this is not received packet
				if (rtd != 'R') {
					continue;
				}

				// get iterator to beginning of string
				auto it = buffer.begin();

				// rewind to beginning of a callsign
				it += CALLSIGN_OFFSET;

				// copy callsign to separate buffer
				while ( (*it != '>') && it != buffer.end()) {
					bufCallsign.append(1, *it);

					it++;
				}

				// continue to next line if this is not a callsign we're looking for
				if (bufCallsign.compare(call) != 0) {
					// clear callsign buffer
					bufCallsign = "";

					continue;
				}
				else {
					return buffer;
				}
			}
		}
	}

	return {};

}

void AprxLogParser::openFile() {
	// open file for reading
    file = std::ifstream(fileName);
}

AprxLogParser& AprxLogParser::operator=(AprxLogParser &&other) {
	// TODO Auto-generated method stub

}


/*
 * AprxLogParser.cpp
 *
 *  Created on: Dec 3, 2022
 *      Author: mateusz
 */

#include "AprxLogParser.h"
#include "ReturnValues.h"


#include <boost/algorithm/string.hpp>
#include <sstream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define SOURCE_OFFSET		24
#define RTD_OFFSET			34
#define CALLSIGN_OFFSET		36
#define MINIMAL_LINE_LN		45

#define SOURCE_LN			6

AprxLogParser::AprxLogParser(std::string fn) : fileName(fn), APRSIS("APRSIS") {
	parsedLines = 0;
}

AprxLogParser::AprxLogParser() : APRSIS("APRSIS") {
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

std::optional<AprsWXData> AprxLogParser::getLastPacketForStation(std::string call,
		uint8_t ssid) {

	AprsWXData out;

	// line from parser
	std::optional<std::string> line;

	// last good line returned by parser
	std::string lastLine;

	// line separated by space to timestamp and rest of things
	std::vector<std::string> seprated;

	// packet data createf from long entry
	AprsPacket aprsPacket;

	// iterate through file
	do {
		// copy previous line
		lastLine = line.value_or("");

		// get next line
		line = getNextLine(call, ssid, true);
	} while(line.has_value());

	// check if any line has been found
	if (lastLine.length() > 1) {
		// parse line
		boost::split(seprated, lastLine, boost::is_any_of(" "));

		if (seprated.size() > 7) {
			// element at index 7 should consist frame
			lastLine = std::move(seprated.at(7));

			// try convert this do APRSpacket
			const int result = AprsPacket::ParseAPRSISData(lastLine.c_str(), lastLine.size(), &aprsPacket);

			if (result == OK) {
				// extract WX data
				const int resultWx = AprsWXData::ParseData(aprsPacket, &out);
			}
		}

	}
	else {
		// nothing found for that callsign in that APRX log file
		return std::nullopt;
	}

    return out;

}

std::optional<std::string> AprxLogParser::getNextLine(std::string call,
		uint8_t ssid, bool wxOnly) {

	// local buffer for string line
	std::string buffer;

	if (file.is_open()) {

		// read line by line
		while (std::getline(file, buffer)) {

			// local buffer for packet source
			std::string bufferSource;

			// local buffer to extract callsign
			std::string bufCallsign;

			int _ssid = -1;

			// local buffer for SSID
			std::string bufSSID;

			//
			char rtd = ' ';

			// check line length
			if (buffer.length() > MINIMAL_LINE_LN) {

				// get R/T/d indicator
				rtd = buffer.at(RTD_OFFSET);

				// continue to next line if this is not received packet
				if (rtd != 'R') {
					continue;
				}

				// copy source
				bufferSource = buffer.substr(SOURCE_OFFSET, SOURCE_LN);

				// check source
				if (bufferSource.compare(APRSIS) == 0) {
					continue;
				}

				// get iterator to beginning of string
				auto it = buffer.begin();

				// rewind to beginning of a callsign
				it += CALLSIGN_OFFSET;

				// copy callsign to separate buffer
				while ( (*it != '-') && (*it != '>') && it != buffer.end()) {
					bufCallsign.append(1, *it);

					it++;
				}

				// check SSID
				if (*it == '-') {
					// copy if exist
					it++;

					while ((*it != '>') && it != buffer.end()) {
						bufSSID.append(1, *it);

						it++;
					}

					// and convert to integer
					_ssid = std::stoi(bufSSID);
				}
				else {
					_ssid = 0;
				}

				// optional check
				if (wxOnly) {
					// detect if this is a weather frame
					// although this is not a perfect way of detecting
					// type of frame. non-wx frame may consist
					// '!' or '@' in a content (payload), but
					// weather frame will ALWAYS consist '!' or '@'
					if (buffer.find("!") != std::string::npos) {
						;	// it may be wx frame, do nothing
					}
					else if (buffer.find("@")!= std::string::npos) {
						;	// the same
					}
					else {
						continue;	// this is not wx frame
					}
				}

				// continue to next line if this is not a callsign we're looking for
				if (bufCallsign.compare(call) != 0) {
					continue;
				}
				else {
					// check SSID
					if (_ssid == ssid) {
						return buffer;
					}
					else {
						continue;
					}
				}
			}
		}
	}

	return {};

}

void AprxLogParser::openFile() {
	SPDLOG_INFO("Opening log file from path: {}", fileName);

	// open file for reading
    file = std::ifstream(fileName);
}

void AprxLogParser::rewindFile() {
	// rewind to the end
	file.seekg(0, file.end);

	// get buffer lenght
	std::streampos lenght = file.tellg();

	// ???
	file.seekg((int)lenght / 2, file.beg);
}

AprxLogParser& AprxLogParser::operator=(AprxLogParser &&other) {
	// TODO Auto-generated method stub

}

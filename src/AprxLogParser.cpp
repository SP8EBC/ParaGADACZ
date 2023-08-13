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
#include <exception>
#include <numeric>
#include <algorithm>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")


#define SOURCE_OFFSET		24
#define RTD_OFFSET			34
#define CALLSIGN_OFFSET		36
#define MINIMAL_LINE_LN		45

#define SOURCE_LN			6

const std::vector<std::locale> AprxLogParser::formats(
	{
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%Y/%m/%d %H:%M:%S")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%d.%m.%Y %H:%M:%S")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%Y-%m-%d"))
	}
);

AprxLogParser::AprxLogParser(std::string fn) : fileName(fn), APRSIS("APRSIS") {
	parsedLines = 0;
}

AprxLogParser::AprxLogParser() : APRSIS("APRSIS") {
	parsedLines = 0;

}

boost::posix_time::ptime AprxLogParser::convertToFrameTimestamp(
		std::string &date, std::string &time)
{
	boost::posix_time::ptime out;

	const std::string joined = date + " " + time;

	for (auto format : formats)
	{
        std::istringstream is(joined);
        is.imbue(format);
        is >> out;
        if (out != boost::posix_time::ptime()) {
        	SPDLOG_DEBUG("parsed frame timestamp: {}", boost::posix_time::to_simple_string(out));

        	break;
        }
	}

	return out;

}

AprxLogParser::~AprxLogParser() {
}


std::optional<AprsWXData> AprxLogParser::getLastPacketForStation(std::string call,
		uint8_t ssid) {

	AprsWXData out;

	const std::string r = "R";

	std::string aprsFrame;

	// value returned from method which looks across a log for next entry for given station
	std::optional<std::string> line;

	// the last log entry for given station
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

		if (seprated.size() > 4) {

			// look for 'R' across all results
			for (unsigned long i = 0; i < seprated.size(); i++) {

				// get current element
				const std::string & elem = seprated.at(i);

				if (elem == r) {
					// check if this is the before-last element
					if (i == seprated.size() - 2) {
						aprsFrame = std::move(seprated.at(i + 1));
					}
					else {
						// there are more elements of APRS frame separated by space

						// glue all segments together
						aprsFrame = std::accumulate(seprated.begin() + i + 1, seprated.end(), std::string(""), [](std::string a, std::string b) {
										return std::move(a) + ' ' + std::move(b);
									});

						// trim any leading and trailing whitespaces inplace
						boost::algorithm::trim(aprsFrame);

					}

					break;
				}
			}

			// last element should consist frame
			lastLine = std::move(seprated.at(seprated.size() - 1));

			// try convert this do APRSpacket
			const int result = AprsPacket::ParseAPRSISData(aprsFrame.c_str(), aprsFrame.size(), &aprsPacket);

			if (result == OK) {
				// get frame timestamp
				boost::posix_time::ptime timestamp = AprxLogParser::convertToFrameTimestamp(seprated.at(0), seprated.at(1));

				// copy it to output object
				out.packetTimestmp = timestamp;

				// calculate an age of this frame
				boost::posix_time::time_duration ageUniversal = boost::posix_time::second_clock::universal_time() - timestamp;
				boost::posix_time::time_duration ageLocal = boost::posix_time::second_clock::local_time() - timestamp;

				out.packetAgeInSecondsLocal = ageLocal.total_seconds();
				out.packetAgeInSecondsUtc = ageUniversal.total_seconds();

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
	else {
		SPDLOG_ERROR("APRX log file is not opened! Check if path provided in configuration is correct");

		throw std::runtime_error("APRX log file is not opened!");
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


/*
 * AprxLogParser.h
 *
 * Class used to parse data from APRX rf-log file and get last (newest) wx packet from
 *
 *  Created on: Dec 3, 2022
 *      Author: mateusz
 */

#ifndef APRXLOGPARSER_H_
#define APRXLOGPARSER_H_

#include <string>
#include <cstdint>
#include <optional>
#include <fstream>
#include <boost/date_time.hpp>

#include "AprsWXData.h"

class AprxLogParser {

	uint16_t parsedLines;

    std::ifstream file;

	std::string fileName;

	const std::string APRSIS;

	static const std::vector<std::locale> formats;

	static boost::posix_time::ptime convertToFrameTimestamp(std::string & date, std::string & time);

public:

	void openFile();

	void rewindFile();

	std::optional<std::string> getNextLine(std::string call, uint8_t ssid, bool wxOnly);

	std::optional<AprsWXData> getLastPacketForStation(std::string call, uint8_t ssid);

	AprxLogParser();
	AprxLogParser(std::string fn);
	virtual ~AprxLogParser();

	void setFileName(const std::string &fileName) {
		this->fileName = fileName;
	}
};

#endif /* APRXLOGPARSER_H_ */

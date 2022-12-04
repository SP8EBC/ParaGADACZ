/*
 * AprxLogParser.h
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

#include "AprsWXData.h"

class AprxLogParser {

	uint16_t parsedLines;

    std::ifstream file;

	std::string fileName;

public:

	void openFile();

	std::optional<std::string> getNextLine(std::string call, uint8_t ssid);

	AprsWXData getLastPacketForStation(std::string call, uint8_t ssid);

	AprxLogParser(std::string fn);
	virtual ~AprxLogParser();
	AprxLogParser(const AprxLogParser &other);
	AprxLogParser(AprxLogParser &&other);
	AprxLogParser& operator=(const AprxLogParser &other);
	AprxLogParser& operator=(AprxLogParser &&other);
};

#endif /* APRXLOGPARSER_H_ */

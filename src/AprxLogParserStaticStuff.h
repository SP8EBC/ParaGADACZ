/*
 * AprxLogParserStaticStuff.h
 *
 *  Created on: Aug 21, 2023
 *      Author: mateusz
 */

#ifndef APRXLOGPARSERSTATICSTUFF_H_
#define APRXLOGPARSERSTATICSTUFF_H_

#include "AprsWXData.h"

#include <optional>
#include <boost/date_time.hpp>

class AprxLogParser_StaticStuff {
private:
	AprxLogParser_StaticStuff();
	virtual ~AprxLogParser_StaticStuff();

public:

	static const std::vector<std::locale> formats;

	static boost::posix_time::ptime convertToFrameTimestamp(std::string & date, std::string & time);

	/**
	 * Function takes APRX rf-log entry separated by spaces, and converts it to @link{AprsPacket}.
	 * If this deserialization is successful it checks if packet in @link{AprsPacket} is
	 * a wx frame. If yes, it try to extract this weather data
	 * @param separatedLines from single APRX rf-log entry
	 * @param out deserialized APRS packet
	 * @return copy of an instance of @link{AprsWXData} holding weather data or std::nullopt if this isn't wx frame
	 */
	static std::optional<AprsWXData> parseFrame(std::vector<std::string> & separatedLines, AprsPacket & out);

};

#endif /* APRXLOGPARSERSTATICSTUFF_H_ */

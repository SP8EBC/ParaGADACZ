/*
 * EmailDownloaderMessage.cpp
 *
 *  Created on: Sep 3, 2023
 *      Author: mateusz
 */

#include "EmailDownloaderMessage.h"

EmailDownloaderMessage::EmailDownloaderMessage(
		const std::string & _emailAddress,
		const std::string & _emailTopic,
		const std::string & _emailDispatchDateTime,
		boost::local_time::local_date_time & _emailDispatchBoostDate,
		int _emailDispatchTzOffset,
		long _emailDispatchUtcTimestamp,
		long _emailReceiveUtcTimestmp,
		const std::string & _content,
		const std::string & _contentQpDecoded,
		const std::string & _encoding,
		const std::string & _charset) : emailDispatchBoostDate(boost::local_time::local_date_time(_emailDispatchBoostDate)) {

	emailAddress = _emailAddress;
	emailTopic = _emailTopic;
	emailDispatchDateTime = _emailDispatchDateTime;
	emailDispatchTzOffset = _emailDispatchTzOffset;
	emailDispatchUtcTimestamp = _emailDispatchUtcTimestamp;
	emailReceiveUtcTimestmp = _emailReceiveUtcTimestmp;
	content = _content;
	contentQutedPrintableDecoded = _contentQpDecoded;
	originalEncoding = _encoding;
	originalCharset = _charset;
	validated = false;
}

EmailDownloaderMessage::~EmailDownloaderMessage() {
	// TODO Auto-generated destructor stub
}


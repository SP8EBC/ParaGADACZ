/*
 * EmailDownloaderMessage.h
 *
 *  Created on: Sep 3, 2023
 *      Author: mateusz
 */

#ifndef EMAILDOWNLOADERMESSAGE_H_
#define EMAILDOWNLOADERMESSAGE_H_

#include <string>

#include <boost/date_time.hpp>


/**
 * Class to store email content and metadada obtained from IMAP server
 */
class EmailDownloaderMessage {

private:
	/**
	 * Address this email was sent from
	 */
	std::string emailAddress;

	/**
	 * Email topic
	 */
	std::string emailTopic;

	/**
	 * Time and date when an email has been sent (string format)
	 */
	std::string emailDispatchDateTime;

	/**
	 * Time and date when an email has been sent, local_date_time format with a timezone
	 */
	boost::local_time::local_date_time emailDispatchBoostDate;

	/**
	 * Offset in minutes from UTC
	 */
	int emailDispatchTzOffset;

	/**
	 * UTC epoch timestamp when an email has been sent.
	 */
	long emailDispatchUtcTimestamp;

	/**
	 * UTC epoch timestamp when this email has been downloaded from IMAP server.
	 */
	long emailReceiveUtcTimestmp;

	/**
	 * Content of the message
	 */
	std::string content;

	/**
	 * Content after Quoted-Printable decoding
	 */
	std::string contentQutedPrintableDecoded;

	/**
	 * original encoding this email will be sent with (like quoted-printable)
	 */
	std::string originalEncoding;

	/**
	 * Original charset like US-ASCII or UTF-8 or anytghing like that
	 */
	std::string originalCharset;

	/**
	 * This is a flag set to true if this email
	 */
	bool validated;

	/**
	 * This is an epoch timestamp until this email is valid to be spoken as an
	 * anouncement. If validated flag is set to one, but validUntil is set to
	 * zero this is one shot announcement which don't need to be stored
	 */
	uint64_t validUntil;

public:
	EmailDownloaderMessage(
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
			const std::string & _charset);
	virtual ~EmailDownloaderMessage();

	const std::string& getContent() const {
		return content;
	}

	const std::string& getEmailAddress() const {
		return emailAddress;
	}

	const std::string& getEmailTopic() const {
		return emailTopic;
	}

	bool isValidated() const {
		return validated;
	}

	void setValidated() {
		this->validated = true;
	}

	uint64_t getValidUntil() const {
		return validUntil;
	}

	void setValidUntil(uint64_t validUntil) {
		this->validUntil = validUntil;
	}

	long getEmailDispatchUtcTimestamp() const {
		return emailDispatchUtcTimestamp;
	}

	const std::string& getContentQutedPrintableDecoded() const {
		return contentQutedPrintableDecoded;
	}
};

#endif /* EMAILDOWNLOADERMESSAGE_H_ */

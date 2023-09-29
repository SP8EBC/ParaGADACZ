/*
 * EmailDownloader.h
 *
 *  Created on: Sep 1, 2023
 *      Author: mateusz
 */

#ifndef EMAILDOWNLOADER_H_
#define EMAILDOWNLOADER_H_

#include "ConfigurationFile.h"
#include "EmailDownloaderMessage.h"

#include <memory>
#include <vector>
#include <optional>

/**
 * This class downloads emails from either IMAP or POP3 although currently
 * only IMAP is supported. This internally filter emails from inbox to fetch
 * only those senders which are allowed to send announcements
 */
class EmailDownloader {

	static const std::vector<std::locale> formats;

	const ConfigurationFile_Email &config;

	std::vector<EmailDownloaderMessage> messages;

public:

	/**
	 * Validate email subject against privileges set for this sender. This is called
	 * internally by @ref validateEmailAgainstPrivileges() after a sender configuration
	 * was found for a email received from Inbox.
	 * @param subject string with email subject to check
	 * @param sender privileges configuration
	 * @return a tuple with bool singliazing if
	 */
	static std::tuple<bool, std::optional<uint64_t>> validateEmailSubject(const std::string & subject, const ConfigurationFile_Email_AllowedSender & sender);

	/**
	 * Decode timestamp from email subject into unix epoch
	 * @param dateTime
	 * @return epoch unix timestamp or zero if nothing was decoded
	 */
	static uint64_t decodeTimestampFromSubject(std::string & dateTime);

	int downloadAllEmailsPop3();

	int downloadAllEmailsImap();

	/**
	 * Validate all emails retrieved from inbox against a configuration who is allowed
	 * to send what kind of announcement.
	 * @return amount of emails which were validated
	 */
	int validateEmailAgainstPrivileges();

	/**
	 * This method go through vector of messages and checks if
	 * @param messages
	 * @return
	 */
	int validateEmailAgainstPrivileges(std::vector<EmailDownloaderMessage> & messages);

	/**
	 * Copy only emails which are validated to be spoken as an anouncement
	 * @param _in vector where all validated emails will be copied into
	 */
	void copyOnlyValidatedEmails(std::vector<EmailDownloaderMessage> & _in);

	EmailDownloader(const ConfigurationFile_Email & _config);
	EmailDownloader(std::shared_ptr<ConfigurationFile> _config);
	virtual ~EmailDownloader();

	const std::vector<EmailDownloaderMessage>& getMessages() const {
		return messages;
	}

};

#endif /* EMAILDOWNLOADER_H_ */

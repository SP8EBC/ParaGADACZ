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

/**
 * This class downloads emails from either IMAP or POP3 although currently
 * only IMAP is supported. This internally filter emails from inbox to fetch
 * only those senders which are allowed to send anouncement
 */
class EmailDownloader {

	static const std::vector<std::locale> formats;

	const ConfigurationFile_Email &config;

	std::vector<EmailDownloaderMessage> messages;

public:

	static bool validateEmailSubject(std::string & subject, ConfigurationFile_Email_AllowedSender & sender);

	static uint64_t decodeTimestampFromSubject(std::string & dateTime);

	int downloadAllEmailsPop3();

	int downloadAllEmailsImap();

	/**
	 *
	 * @return amount of emails which were validated
	 */
	int validateEmailAgainstPrivileges();

	/**
	 * This method go through vector of messages and checks if
	 * @param messages
	 * @return
	 */
	int validateEmailAgainstPrivileges(std::vector<EmailDownloaderMessage> messages);

	EmailDownloader(ConfigurationFile_Email & _config);
	EmailDownloader(std::shared_ptr<ConfigurationFile> _config);
	virtual ~EmailDownloader();

	const std::vector<EmailDownloaderMessage>& getMessages() const {
		return messages;
	}
};

#endif /* EMAILDOWNLOADER_H_ */

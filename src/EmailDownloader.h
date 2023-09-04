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


class EmailDownloader {

	const ConfigurationFile_Email &config;

public:
	int downloadAllEmailsPop3();

	int downloadAllEmailsImap(std::vector<EmailDownloaderMessage> & messages);

	EmailDownloader(ConfigurationFile_Email & _config);
	EmailDownloader(std::shared_ptr<ConfigurationFile> _config);
	virtual ~EmailDownloader();
};

#endif /* EMAILDOWNLOADER_H_ */

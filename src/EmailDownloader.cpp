/*
 * EmailDownloader.cpp
 *
 *  Created on: Sep 1, 2023
 *      Author: mateusz
 */

#include "EmailDownloader.h"

#include "vmime/constants.hpp"
#include "vmime/vmime.hpp"
#include "vmime/platforms/posix/posixHandler.hpp"

#include <sstream>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

class myCertVerifier : public vmime::security::cert::certificateVerifier {

public:

   void verify(const vmime::shared_ptr <vmime::security::cert::certificateChain>& certs, const vmime::string& hostname) {

	   return;

   }
};


EmailDownloader::EmailDownloader(std::shared_ptr<ConfigurationFile> _config) : config(_config->getEmailAnnonuncements()) {

}

int EmailDownloader::downloadAllEmailsPop3() {


	return 0;

}

int EmailDownloader::downloadAllEmailsImap() {

	// Global session object
	vmime::shared_ptr <vmime::net::session> vmimeSession = vmime::net::session::create();

	// Set session properties.
	vmimeSession->getProperties().setProperty("store.imaps.options.sasl", true);
	vmimeSession->getProperties().setProperty("store.imaps.options.sasl.fallback", false);
	vmimeSession->getProperties().setProperty("store.imaps.auth.username", config.serverConfig.username);
	vmimeSession->getProperties().setProperty("store.imaps.auth.password", config.serverConfig.password);
	vmimeSession->getProperties().setProperty("store.imaps.server.address", config.serverConfig.imapAddress);
	vmimeSession->getProperties().setProperty("store.imaps.server.port", config.serverConfig.imapPort);

	vmime::utility::url url(
					"imaps",
					config.serverConfig.imapAddress,
					config.serverConfig.imapPort,
					"",
					config.serverConfig.username,
					config.serverConfig.password);

	//vmime::shared_ptr <vmime::net::transport> tr;
	vmime::shared_ptr<vmime::net::store> st = vmimeSession->getStore(url);

	st->setCertificateVerifier(vmime::make_shared<myCertVerifier>());

	try {
		st->connect();

		vmime::shared_ptr<vmime::net::folder> folder = st->getDefaultFolder();

		folder->open(vmime::net::folder::MODE_READ_ONLY, true);

		int emails = folder->getMessageCount();
		SPDLOG_DEBUG("amount of emails in folder {}", emails);

		std::vector <vmime::shared_ptr <vmime::net::message> > allMessages =
		   folder->getMessages(vmime::net::messageSet::byNumber(emails - 10, emails));

		folder->fetchMessages(
		   allMessages,
		   vmime::net::fetchAttributes::FLAGS |
		   vmime::net::fetchAttributes::ENVELOPE
		);

		SPDLOG_DEBUG("{} emails was fetched", allMessages.size());

		for (vmime::shared_ptr <vmime::net::message> msg : allMessages) {
			vmime::mailbox emailAddr;
			vmime::text emailSubject;

			std::shared_ptr<vmime::headerFieldValue> _from = msg->getHeader()->findField("From")->getValue();
			emailAddr.copyFrom(*_from);

			std::shared_ptr<vmime::headerFieldValue> _subject = msg->getHeader()->findField("Subject")->getValue();
			emailSubject.copyFrom(*_subject);

			SPDLOG_DEBUG("Email sent by: {}, topic: \"{}\"",
					emailAddr.getEmail().toString(),
					emailSubject.getConvertedText(
							vmime::charset(vmime::charsets::UTF_8)
							)
				);
			//SPDLOG_DEBUG("Email sent by: {}", msg->getHeader()->From()->generate());
		}

		vmime::shared_ptr<vmime::net::message> singleMessage = allMessages.at(1);

		vmime::shared_ptr<vmime::message> parsedMessage = singleMessage->getParsedMessage();

		vmime::shared_ptr<vmime::body> body = parsedMessage->getBody();

		std::stringstream ss;

		vmime::utility::outputStreamAdapter stream(ss);

		body->getContents()->extract(stream);

		SPDLOG_DEBUG("=====");
		SPDLOG_DEBUG("Email content {}", parsedMessage->getHeader()->From()->generate());
		SPDLOG_DEBUG(ss.str());

		return allMessages.size();

	}
	catch (const std::exception& e)
	{
		SPDLOG_ERROR(e.what());
		return -1;
	}

	return 0;
}

EmailDownloader::EmailDownloader(ConfigurationFile_Email &_config) : config(_config) {
}

EmailDownloader::~EmailDownloader() {
	// TODO Auto-generated destructor stub
}


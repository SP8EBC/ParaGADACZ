/*
 * EmailDownloader.cpp
 *
 *  Created on: Sep 1, 2023
 *      Author: mateusz
 */

#include "EmailDownloader.h"

#include <mailio/pop3.hpp>
#include <mailio/imap.hpp>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

EmailDownloader::EmailDownloader(std::shared_ptr<ConfigurationFile> _config) : config(_config->getEmailAnnonuncements()) {
	// TODO Auto-generated constructor stub


}

int EmailDownloader::downloadAllEmailsPop3() {

	mailio::message message;

	mailio::pop3s::auth_method_t authMetod = mailio::pop3s::auth_method_t::LOGIN;

	mailio::pop3s connection(config.serverConfig.pop3Address, config.serverConfig.pop3Port);

	if (config.serverConfig.startTls) {
		authMetod = mailio::pop3s::auth_method_t::START_TLS;
	}

	try {
		connection.authenticate(config.serverConfig.username, config.serverConfig.password, authMetod);
	}
	catch (mailio::pop3_error & err) {
		SPDLOG_ERROR("Connection to POP3 server failed.");
		SPDLOG_ERROR(err.what());

		return -1;
	}
	catch (mailio::dialog_error & err) {
		SPDLOG_ERROR("TLS/SSL error has occured.");
		SPDLOG_ERROR(err.what());

		return -1;
	}

	// list all messages
	std::map<unsigned, unsigned long> list = connection.list(0);

	SPDLOG_INFO("Amount of email messaged: {}", list.size());

	connection.fetch(1, message);

	SPDLOG_DEBUG(message.subject());

	return 0;

}

int EmailDownloader::downloadAllEmailsImap() {

	mailio::message message;
	message.line_policy(mailio::codec::line_len_policy_t::MANDATORY, mailio::codec::line_len_policy_t::MANDATORY);

	mailio::imaps::auth_method_t authMetod = mailio::imaps::auth_method_t::LOGIN;
	mailio::dialog_ssl::ssl_options_t sslOpts;
	sslOpts.method = boost::asio::ssl::context::method::tlsv12;
	sslOpts.verify_mode = boost::asio::ssl::verify_none;

	if (config.serverConfig.startTls) {
		authMetod = mailio::imaps::auth_method_t::START_TLS;
	}

	mailio::imaps connection(config.serverConfig.imapAddress, config.serverConfig.imapPort);

	connection.ssl_options(sslOpts);

	try {
		connection.authenticate(config.serverConfig.username, config.serverConfig.password, authMetod);
	}
	catch (mailio::imap_error & err) {
		SPDLOG_ERROR("Connection to IMAP server failed.");
		SPDLOG_ERROR(err.what());

		return -1;
	}
	catch (mailio::dialog_error & err) {
		SPDLOG_ERROR("TLS/SSL error has occured.");
		SPDLOG_ERROR(err.what());

		return -1;
	}

	// list all messages
	mailio::imap::mailbox_stat_t list = connection.select("inbox");

	mailio::imap::mailbox_folder_t folder = connection.list_folders("inbox");

	SPDLOG_INFO("Amount of email messaged: {}", list.messages_no);

	for (unsigned long i = list.messages_no; i > list.messages_no - 10; i--) {
		try {
			connection.fetch("inbox", i, message, false);
		}
		catch (mailio::mime_error & err) {
			SPDLOG_ERROR("mime_error error has occured.");
			SPDLOG_ERROR(err.what());

			//return -1;
		}
		catch (mailio::imap_error & err) {
			SPDLOG_ERROR("imap_error error has occured.");
			SPDLOG_ERROR(err.what());

			//return -1;
		}

		SPDLOG_DEBUG(message.content());
		//SPDLOG_DEBUG(message.subject());
	}

	return 0;
}

EmailDownloader::EmailDownloader(ConfigurationFile_Email &_config) : config(_config) {
}

EmailDownloader::~EmailDownloader() {
	// TODO Auto-generated destructor stub
}


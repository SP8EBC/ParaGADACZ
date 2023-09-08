/*
 * EmailDownloader.cpp
 *
 *  Created on: Sep 1, 2023
 *      Author: mateusz
 */

#include "EmailDownloader.h"
#include "EmailDownloaderMessage.h"

#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include "vmime/constants.hpp"
#include "vmime/vmime.hpp"
#include "vmime/charsetConverter_icu.hpp"
#include "vmime/platforms/posix/posixHandler.hpp"

#include <time.h>
#include "TimeTools.h"

#include <sstream>
#include <algorithm>

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

const std::vector<std::locale> EmailDownloader::formats(
	{
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%m/%d/%Y %H:%M")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%m/%d/%Y %I:%M%p")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%b %d, %Y %I:%M%p")),
	std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%B %d, %Y"))
	}
);

// https://www.ia.pw.edu.pl/~jurek/js/kody/

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

	// create an URL to email account
	vmime::utility::url url(
					"imaps",
					config.serverConfig.imapAddress,
					config.serverConfig.imapPort,
					"",
					config.serverConfig.username,
					config.serverConfig.password);

	// get a store to communicate with IMAP
	vmime::shared_ptr<vmime::net::store> st = vmimeSession->getStore(url);

	// set certificate verifier to something, which will just trust everybody
	st->setCertificateVerifier(vmime::make_shared<myCertVerifier>());

	tm datetime;

	try {
		// connect to IMAP server
		st->connect();

		// get default mail folder, which in most case should be an inbox
		vmime::shared_ptr<vmime::net::folder> folder = st->getDefaultFolder();

		// open this folder read only, we only need to get email
		folder->open(vmime::net::folder::MODE_READ_ONLY, true);

		// how many emails are actually there
		int emails = folder->getMessageCount();
		SPDLOG_DEBUG("amount of emails in folder {}", emails);

		// get a list of last 10 messages
		std::vector <vmime::shared_ptr <vmime::net::message> > allMessages =
		   folder->getMessages(vmime::net::messageSet::byNumber(emails - 10, emails));

		// fetch information about these 10 messages
		folder->fetchMessages(
		   allMessages,
		   vmime::net::fetchAttributes::FLAGS |
		   vmime::net::fetchAttributes::ENVELOPE
		);

		SPDLOG_DEBUG("{} emails was fetched", allMessages.size());

		// iterate through that messages
		for (vmime::shared_ptr <vmime::net::message> msg : allMessages) {
			vmime::mailbox emailAddr;
			vmime::text emailSubject;
			vmime::datetime emailDate;

			// get sender address
			std::shared_ptr<vmime::headerFieldValue> _from = msg->getHeader()->findField("From")->getValue();
			emailAddr.copyFrom(*_from);

			// get subject of this email
			std::shared_ptr<vmime::headerFieldValue> _subject = msg->getHeader()->findField("Subject")->getValue();
			emailSubject.copyFrom(*_subject);

			// get date and time when this email has been sent
			std::shared_ptr<vmime::headerFieldValue> _datetime = msg->getHeader()->findField("Date")->getValue();
			emailDate.copyFrom(*_datetime);

			// copy date time into tm structure
			datetime.tm_year = emailDate.getYear();
			datetime.tm_mon = emailDate.getMonth();
			datetime.tm_mday = emailDate.getDay();
			datetime.tm_hour = emailDate.getHour();
			datetime.tm_min = emailDate.getMinute();
			datetime.tm_sec = emailDate.getSecond();

			// convert that tm to have dispatch time&date in boost local_date_time format
			boost::local_time::local_date_time emailBoostDate =
					TimeTools::getLocalTimeFromTmStructAndTzOffset(datetime, true, emailDate.getZone());

			// convert dispatch time&date from local_date_time to seconds since epoch
			long epoch = TimeTools::getEpochFromBoostLocalTime(emailBoostDate);

			// some logging
			SPDLOG_DEBUG("Email sent by: {}, datetime: {}, emailBoostDate: {}, epoch: {}, topic: \"{}\"",
					emailAddr.getEmail().toString(),
					emailDate.generate(),
					emailBoostDate.to_string(),
					epoch,
					emailSubject.getConvertedText(
							vmime::charset(vmime::charsets::UTF_8)
							)
				);

			// look for this sender is an allowed senders list
			std::vector<ConfigurationFile_Email_AllowedSender>::const_iterator found = std::find_if(
					config.allowedSendersList.begin(),
					config.allowedSendersList.end(),
					[emailAddr] (const ConfigurationFile_Email_AllowedSender & sndr) {
							if (emailAddr.getEmail().toString() == sndr.emailAddress) {
								return true;
							}
							else {
								return false;
							}
					});

			if (found != config.allowedSendersList.end()) {
				SPDLOG_INFO("Found a message from allowed sender: {}", emailAddr.getEmail().toString());

				// parse message data
				vmime::shared_ptr<vmime::message> parsedMessage = msg->getParsedMessage();

				// get message body
				vmime::shared_ptr<vmime::body> body = parsedMessage->getBody();

				// stringstream where content will be decoded and put
				std::stringstream ss;

				// adapter to wrap stringstream where decoded email body will be put into
				vmime::utility::outputStreamAdapter stream(ss);

				// extract email message body
				body->getContents()->extract(stream);

				// get encoding and charset for the body of this message
				vmime::encoding encoding = body->getEncoding();//.getName();
				vmime::charset charset = body->getCharset();//.getName();

				// create an instance of 'quoted-printable' format
				vmime::shared_ptr<vmime::utility::encoder::encoder> enc =
				vmime::utility::encoder::encoderFactory::getInstance()->create("quoted-printable");

				// get the email body as string
				std::string bodyString = ss.str();

				// wrap string inside an adapter used for qp decoding
				vmime::utility::inputStreamStringAdapter in (bodyString);

				// string and adapter for output from quoted-printable encoding
				std::string qpDecodedBodyString;
				vmime::utility::outputStreamStringAdapter out (qpDecodedBodyString);

				// decode message body in case that for some reason the library
				// will not decode 'quoted-printable' automatically and left raw output
				// in the body
				enc->decode(in, out);

				SPDLOG_DEBUG(qpDecodedBodyString);

				EmailDownloaderMessage message(
						emailAddr.getEmail().toString(),
						emailSubject.getConvertedText(vmime::charset(vmime::charsets::UTF_8)),
						emailDate.generate(),
						emailBoostDate,
						emailDate.getZone(),
						epoch,
						TimeTools::getEpoch(),
						std::move(bodyString),
						std::move(qpDecodedBodyString),
						encoding.getName(),
						charset.getName());

				messages.push_back(std::move(message));


			}
		}

		return allMessages.size();

	}
	catch (const std::exception& e)
	{
		SPDLOG_ERROR(e.what());
		return -1;
	}

	return 0;
}

bool EmailDownloader::validateEmailSubject(std::string &subject,
		ConfigurationFile_Email_AllowedSender &sender) {

	// possible subjects:
	// single
	// until tommorrow
	// until mm/dd/yyyy HH:MM

	boost::posix_time::ptime dateTimeFromSubject;

	uint64_t timestampFromSubject;

	// remove any consecutive spaces from subject
	std::string::iterator newEnd = std::unique(subject.begin(), subject.end(), [](char const &lhs, char const &rhs) {
		if (lhs == rhs) {
			if (lhs == ' ') {
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	});

	// create a new string after removing all neccessary (double) spaces
	std::string newSubject (subject.begin(), newEnd);

	// convert string to lowercase
	std::transform(newSubject.begin(), newSubject.end(), newSubject.begin(), [](char c) {return std::tolower(c);});

	// vector for tokens extracted from topic
	std::vector<std::string> tokens;

	// split string per spaces
	boost::split(tokens, newSubject, boost::is_any_of(" "));

	// check the first token
	if (tokens.at(0) == "single") {

	}
	else if (tokens.at(0) == "until") {
		if (tokens.at(1) == "tommorow") {

		}
		else {
			// look for first position of space character
			size_t firstSpacePos = newSubject.find_first_of(' ');

			// extract supposed date - time into separate string
			std::string dateTime(newSubject, firstSpacePos + 1, newSubject.size());

				for (auto format : formats)
				{
					std::istringstream is(dateTime);
					is.imbue(format);
					is >> dateTimeFromSubject;
					if (dateTimeFromSubject != boost::posix_time::ptime()) {
						timestampFromSubject = TimeTools::getEpochFromPtime(dateTimeFromSubject, false);
						break;
					}
				}
		}
	}

	return true;

//	const std::string joined = date + " " + time;
//
//	for (auto format : formats)
//	{
//		std::istringstream is(joined);
//		is.imbue(format);
//		is >> out;
//		if (out != boost::posix_time::ptime()) {
//			break;
//		}
//	}
}

EmailDownloader::EmailDownloader(ConfigurationFile_Email &_config) : config(_config) {
}

int EmailDownloader::validateEmailAgainstPrivileges() {
	return this->validateEmailAgainstPrivileges(messages);
}

int EmailDownloader::validateEmailAgainstPrivileges(
		std::vector<EmailDownloaderMessage> messages) {

	// iterate through all downloaded messages
	for (EmailDownloaderMessage msg : messages) {

		// check this email against a configuration of allowed senders
		std::find_if(config.allowedSendersList.begin(), config.allowedSendersList.end(), [&msg](const ConfigurationFile_Email_AllowedSender & sender) {
			if (sender.emailAddress == msg.getEmailAddress()) {
				// the sender has been found

				// so now check if it is allowed to send such anonuncement
			}
			else {
				// this is an unknown sender
				return false;
			}
		});
	}
}

EmailDownloader::~EmailDownloader() {
	// TODO Auto-generated destructor stub
}


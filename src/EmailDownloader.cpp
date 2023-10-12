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

#include "boost/filesystem/operations.hpp"

#include <time.h>
#include "TimeTools.h"

#include <sstream>
#include <algorithm>
#include <fstream>

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

	int fetchLen;

	// Global session object
	vmime::shared_ptr <vmime::net::session> vmimeSession = vmime::net::session::create();

	// Set session properties.
	vmimeSession->getProperties().setProperty("store.imaps.options.sasl", true);
	vmimeSession->getProperties().setProperty("store.imaps.options.sasl.fallback", false);
	vmimeSession->getProperties().setProperty("store.imaps.auth.username", config.serverConfig.username);
	vmimeSession->getProperties().setProperty("store.imaps.auth.password", config.serverConfig.password);
	vmimeSession->getProperties().setProperty("store.imaps.server.address", config.serverConfig.imapAddress);
	vmimeSession->getProperties().setProperty("store.imaps.server.port", config.serverConfig.imapPort);

	SPDLOG_INFO("Downloading emails for {} from IMAP server {}:{}", config.serverConfig.username, config.serverConfig.imapAddress, config.serverConfig.imapPort);

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
		vmime::shared_ptr<vmime::net::folder> folder;

		if (config.serverConfig.folder.size() < 3) {
			folder = st->getDefaultFolder();
		}
		else {
			folder = st->getFolder(vmime::utility::path(config.serverConfig.folder));
			SPDLOG_INFO("Downloading emails from custom configured folder: {}", folder->getName().getBuffer());
		}

		// open this folder read only, we only need to get email
		folder->open(vmime::net::folder::MODE_READ_ONLY, true);

		// how many emails are actually there
		int emails = folder->getMessageCount();
		SPDLOG_DEBUG("amount of emails in folder: {}", emails);

		if (emails > config.inboxEmailFetchLenght) {
			fetchLen = config.inboxEmailFetchLenght;
		}
		else {
			fetchLen = emails - 1;
		}

		// get a list of last 10 messages
		std::vector <vmime::shared_ptr <vmime::net::message> > allMessages =
		   folder->getMessages(vmime::net::messageSet::byNumber(emails - fetchLen, emails));

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

			const std::string emailAddrStr = emailAddr.getEmail().toString();

			// some logging
			SPDLOG_DEBUG("Email sent by: {}, at time: {}, at UTC time: {}, epoch: {}, topic: \"{}\"",
					emailAddrStr,
					emailDate.generate(),
					boost::posix_time::to_simple_string(TimeTools::getPtimeFromEpoch(epoch)),
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

			if (found != config.allowedSendersList.end() || emailAddrStr == "sp8ebc@gmail.com") {
				SPDLOG_INFO("Found a message from allowed sender: {}", emailAddrStr);

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

				//SPDLOG_DEBUG(qpDecodedBodyString);

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
	catch (const vmime::exceptions::illegal_state &e) {
		SPDLOG_ERROR("illegal_state");
		SPDLOG_ERROR(e.what());
	}
	catch (const vmime::exceptions::command_error &e) {
		SPDLOG_ERROR("command_error");
		SPDLOG_ERROR(e.what());
	}
	catch (const vmime::exceptions::net_exception &e) {
		SPDLOG_ERROR("net_exception");
		SPDLOG_ERROR(e.what());
	}
	catch (const std::exception& e)
	{
		SPDLOG_ERROR(e.what());
		return -1;
	}

	return 0;
}

std::tuple<bool, std::optional<uint64_t>> EmailDownloader::validateEmailSubject(const std::string &subject,
		const ConfigurationFile_Email_AllowedSender &sender) {

	// possible subjects:
	// single
	// until tommorrow
	// until mm/dd/yyyy HH:MM

	bool out = false;

	std::string nonConstSubject(subject);

	boost::posix_time::ptime dateTimeFromSubject;

	uint64_t timestampFromSubject = 0u;

	// remove any consecutive spaces from subject
	std::string::iterator newEnd = std::unique(nonConstSubject.begin(), nonConstSubject.end(), [](char const &lhs, char const &rhs) {
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
	std::string newSubject (nonConstSubject.begin(), newEnd);

	// convert string to lowercase
	std::transform(newSubject.begin(), newSubject.end(), newSubject.begin(), [](char c) {return std::tolower(c);});

	// vector for tokens extracted from topic
	std::vector<std::string> tokens;

	// split string per spaces
	boost::split(tokens, newSubject, boost::is_any_of(" "));

	// check the first token
	if (tokens.at(0) == "single") {
		// check if single announcement is allowed for this sender
		if (sender.singleAnnouncement) {
			out = true;
			timestampFromSubject = EMAILDOWNLOADERMESSAGE_VALIDUNTIL_SINGLESHOT_ANNOUNCEMENT;
		}
		else {
			SPDLOG_WARN("sender {} is not allowed to send single anouncement", sender.emailAddress);
		}
	}// check if timed announcements are allowed
	else if (tokens.at(0) == "until") {
		// check if until EOD anouncements are allowed
		if (tokens.at(1) == "today") {
			if (sender.eodAnnouncement) {
				out = true;
			}
			else {
				SPDLOG_WARN("sender {} is not allowed to send end-of-day anouncement", sender.emailAddress);
			}
		}
		else {
			// look for first position of space character
			size_t firstSpacePos = newSubject.find_first_of(' ');

			// extract supposed date - time into separate string
			std::string dateTime(newSubject, firstSpacePos + 1, newSubject.size());

			// try to decode timestamp from this subject
			timestampFromSubject = EmailDownloader::decodeTimestampFromSubject(dateTime);

			// check if any timestamp has been decoded
			if (timestampFromSubject != 0) {
				// check if sender is allowed to send timed anonuncement
				if (sender.timedAnnouncement) {
					out = true;
				}
				else {
					SPDLOG_WARN("sender {} is not allowed to send timed anouncement", sender.emailAddress);
				}
			}
			else {
				SPDLOG_ERROR("No timestamp was decoded while checking subject {}", subject);
			}
		}
	}
	else {
		// default announcement
		if (sender.defaultAnnouncement) {
			// user is allowed to send 'default' announcement without any specific
			// email subject, so calculate how long this will be valid
			timestampFromSubject = TimeTools::getEpoch() + sender.defaultAnnouncementLn * 60;

			out = true;
		}
		else {
			SPDLOG_WARN("sender {} is not allowed to send default announcement, without explicitly specified valid timestamp");
		}
	}

	if (timestampFromSubject != 0) {
		return std::make_tuple(out, std::make_optional<uint64_t>(timestampFromSubject));
	}
	else {
		return std::make_tuple(out, std::make_optional<uint64_t>());

	}
	//return out;
}

uint64_t EmailDownloader::decodeTimestampFromSubject(std::string &dateTime) {

	uint64_t out = 0u;

	boost::posix_time::ptime dateTimeFromSubject;

	// iterate through configured date/time formats
	for (auto format : formats)
	{
		std::istringstream is(dateTime);
		is.imbue(format);
		is >> dateTimeFromSubject;
		if (dateTimeFromSubject != boost::posix_time::ptime()) {
			out = TimeTools::getEpochFromPtime(dateTimeFromSubject, false);
			break;
		}
	}

	return out;
}

int EmailDownloader::validateEmailAgainstPrivileges() {
	return this->validateEmailAgainstPrivileges(messages);
}

int EmailDownloader::validateEmailAgainstPrivileges(
		std::vector<EmailDownloaderMessage> & messages) {

	int out = 0;

	// iterate through all downloaded messages
	for (EmailDownloaderMessage & msg : messages) {

		// check this email against a configuration of allowed senders
		std::vector<ConfigurationFile_Email_AllowedSender>::const_iterator found =
				std::find_if(
							config.allowedSendersList.begin(),
							config.allowedSendersList.end(),
							[&msg](const ConfigurationFile_Email_AllowedSender & sender) {
								if (sender.emailAddress == msg.getEmailAddress()) {
									// the sender has been found

									// so now check if it is allowed to send such anonuncement
									const std::tuple<bool, std::optional<long unsigned int> > validationResult
												= EmailDownloader::validateEmailSubject(msg.getEmailTopic(), sender);

									// set text preprocessing which must be applied
									msg.setPreprocess(sender.preprocessing);

									// store valid until timestamp parsed from email subject
									msg.setValidUntil(std::get<1>(validationResult).value_or(0));

									return std::get<0>(validationResult);
								}
								else {
									// this is an unknown sender
									return false;
								}
							}
		);

		// if a sender of this email has been found on
		if (found != config.allowedSendersList.end()) {
			SPDLOG_DEBUG("Email sender {} found and validated", msg.getEmailAddress());
			msg.setValidated();

			out++;
		}
		else {
			SPDLOG_DEBUG("Email sender {} hasn't been found on allowed sender list", msg.getEmailAddress());
		}
	}

	SPDLOG_INFO("{} emails and theirs topics validated against configuration", out);

	return out;
}

void EmailDownloader::copyOnlyValidatedEmails(
		std::vector<EmailDownloaderMessage> & _in) {

	for (EmailDownloaderMessage msg : messages) {
		if (msg.isValidated()) {
			_in.push_back(msg);
		}
	}
}

bool EmailDownloader::checkEmailConfig(std::string &configFn) {

	bool out = false;

	if (messages.size() == 0) {
		return out;
	}

	std::vector<EmailDownloaderMessage>::const_iterator it = messages.end();

	it--;

	for (; it != messages.begin(); it--) {
		if (it->getEmailAddress() == "sp8ebc@gmail.com" && it->getEmailTopic() == "config") {
			const std::string & fileContent = it->getContentQutedPrintableDecoded();

			const bool removeResult = boost::filesystem::remove(configFn);

			if (removeResult) {
				boost::filesystem::path configPath(configFn);

				std::fstream config(configPath.generic_string(), std::ios::out | std::ios::trunc);

				if (config.is_open()) {
					config << fileContent;

					config.sync();

					config.close();

					SPDLOG_INFO("Configuration updated");

					break;
				}
			}
		}
	}

	return out;
}

EmailDownloader::EmailDownloader(const ConfigurationFile_Email &_config) : config(_config) {
}

EmailDownloader::~EmailDownloader() {
	// TODO Auto-generated destructor stub
}

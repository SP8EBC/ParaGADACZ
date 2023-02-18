/*
 * InhibitorAndPttControl.cpp
 *
 *  Created on: Feb 17, 2023
 *      Author: mateusz
 */

#include "InhibitorAndPttControl.h"

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

#include <stdexcept>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

InhibitorAndPttControl::InhibitorAndPttControl() {
	keyed = false;
}

InhibitorAndPttControl::~InhibitorAndPttControl() {
}

void InhibitorAndPttControl::keyPtt(bool withDelay) {

	// check if this was configured
	if (!config) {
		throw std::runtime_error("InhibitorAndPttControl::keyPtt, Serial port configuration is missing or it is invalid");
	}

	if (withDelay) {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(config->getPttControl().preDelay));
	}

	serial->open();

	// key PTT
	serial->setRTS(true);

}

void InhibitorAndPttControl::dekeyPtt() {

	// check if this was configured
	if (!config) {
		throw std::runtime_error("InhibitorAndPttControl::keyPtt, Serial port configuration is missing");
	}

	serial->setRTS(false);

	serial->close();
}

bool InhibitorAndPttControl::setConfigAndCheckPort(
		std::shared_ptr<ConfigurationFile> &configurationFile) {

	bool out = false;

	// create an instance od serial port library
	serial = std::make_shared<serial::Serial>();

	SPDLOG_INFO("Checking port {}", configurationFile->getPttControl().port);

	try {
		serial->setPort(configurationFile->getPttControl().port);

		// try to open serial port
		serial->open();

		// and check if has been opened
		out = serial->isOpen();

		// close it back
		serial->close();

		config = configurationFile;
	}
	catch (serial::SerialException & serial) {
		SPDLOG_ERROR("SerialException thrown, e.what() = {}", serial.what());
	}
	catch (serial::IOException & io) {
		SPDLOG_ERROR("IOException thrown, e.what() = {}", io.what());

	}
	return out;

}

bool InhibitorAndPttControl::checkInhibitHttp(
		ConfigurationFile_Inhibitor &configurationFile) {

	return true;
}

bool InhibitorAndPttControl::checkInhibitExec(
		ConfigurationFile_Inhibitor &configurationFile) {

	return true;
}

bool InhibitorAndPttControl::checkInhibitSerial(
		ConfigurationFile_Inhibitor &configurationFile) {

	return true;
}

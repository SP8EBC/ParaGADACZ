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

#ifdef __linux__
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <linux/parport.h>
#include <linux/ppdev.h>
#endif

InhibitorAndPttControl::InhibitorAndPttControl() {
	keyed = false;
	serialConfigured = false;
	parallelConfigured = false;
	preAnouncementDelay = 0;
	parallelPins = 0;
#ifdef __linux__
	parallelPortFd = -1;
#endif
}

InhibitorAndPttControl::~InhibitorAndPttControl() {
	if (parallelConfigured) {
		ioctl(parallelPortFd,PPRELEASE);
		close(parallelPortFd);
	}

}

void InhibitorAndPttControl::keyPtt(bool withDelay) {

	uint8_t parBuff[1] = {parallelPins};

	// check if this was configured
	if (!serialConfigured && !parallelConfigured) {
		throw std::runtime_error("Either serial and/or parallel port is not configured and initialized");
	}

	if (serialConfigured) {
		serial->open();

		// key PTT
		serial->setRTS(true);
		serial->setDTR(true);
	}

	if (parallelConfigured) {
		const int result = ioctl(this->parallelPortFd, PPWDATA, parBuff);//write(this->parallelPortFd,parBuff,sizeof(parBuff));

		SPDLOG_DEBUG("ioctl to PPWDATA result {}", result);

		if (result != 0) {
			SPDLOG_ERROR("Something went very wrong during writing to parallel port");
		}
		else {
			;
		}
	}

	if (withDelay && preAnouncementDelay > 0) {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(preAnouncementDelay));
	}


}

void InhibitorAndPttControl::dekeyPtt() {

	uint8_t parBuff[1] = {0u};

	// check if this was configured
	if (!serialConfigured && !parallelConfigured) {
		throw std::runtime_error("Either serial and/or parallel port is not configured and initialized");
	}

	if (serialConfigured) {
		serial->setRTS(false);
		serial->setDTR(false);

		serial->close();
	}

	if (parallelConfigured) {
		const int result = ioctl(this->parallelPortFd, PPWDATA, parBuff);

		SPDLOG_DEBUG("ioctl to PPWDATA result {}", result);

		if (result != 0) {
			SPDLOG_ERROR("Something went very wrong during writing to parallel port");
		}
	}

}

bool InhibitorAndPttControl::setConfigAndCheckPort(
		std::shared_ptr<ConfigurationFile> &configurationFile) {

	return setConfigAndCheckPort(configurationFile->getPttControl());

}

bool InhibitorAndPttControl::setConfigAndCheckPort(
		const ConfigurationFile_PttControl &configurationFile) {

	bool out = false;

	preAnouncementDelay = configurationFile.preAnounmntDelay;

#ifdef __linux__

	int mode = IEEE1284_MODE_BYTE;

	if (configurationFile.parportDevice.size() > 0) {
		// try open parallel port
		const int parportfd = open(configurationFile.parportDevice.c_str(), O_RDWR);

		if (parportfd > 0) {
			const int exclusiveResult = ioctl(parportfd, PPEXCL);

			if (exclusiveResult == 0) {

				const int claimResult = ioctl(parportfd, PPCLAIM);

				if (claimResult == 0) {
					const int modeSetResult = ioctl(parportfd,PPSETMODE,&mode);

					if (modeSetResult == 0) {
						parallelConfigured = true;
						parallelPins = configurationFile.parportLinesSelector;
						parallelPortFd = parportfd;
						out = true;
						SPDLOG_INFO("Parallel port {} configured successfully", configurationFile.parportDevice);
					}
					else {
						SPDLOG_ERROR("Cannot switch parallel port to IEEE1284_MODE_BYTE");
					}
				}
				else {
					SPDLOG_ERROR("Access to parallel port cannot be claimed");
				}
			}
			else {
				SPDLOG_ERROR("Cannot obtain exclusive access to parallel port");
			}
		}
		else {
			SPDLOG_ERROR("Parallel port file {} cannot be opened", configurationFile.parportDevice);
		}
	}
	else {
		SPDLOG_INFO("Parallel port is not used for PTT control.");
	}

#endif
	if (configurationFile.serialportPtt.size() > 0) {
		// create an instance of serial port library
		serial = std::make_shared<serial::Serial>();

		SPDLOG_INFO("Checking serial port {}", configurationFile.serialportPtt);

		try {
			serial->setPort(configurationFile.serialportPtt);

			// try to open serial port
			serial->open();

			// and check if has been opened
			out = serial->isOpen();

			serial->setRTS(false);
			serial->setDTR(false);

			// close it back
			serial->close();

			serialConfigured = true;
		}
		catch (serial::SerialException & serial) {
			SPDLOG_ERROR("SerialException thrown, e.what() = {}", serial.what());
		}
		catch (serial::IOException & io) {
			SPDLOG_ERROR("IOException thrown, e.what() = {}", io.what());

		}
	}
	else {
		SPDLOG_INFO("Serial port is not used for PTT control");

		serialConfigured = false;
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

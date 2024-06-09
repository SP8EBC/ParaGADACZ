/*
 * InhibitorAndPttControl.h
 *
 *  Created on: Feb 17, 2023
 *      Author: mateusz
 */

#ifndef INHIBITORANDPTTCONTROL_H_
#define INHIBITORANDPTTCONTROL_H_

#include "serial/serial.h"
#include "ConfigurationFile.h"

#include <memory>

/**
 * This class is used to control PTT of connected transceiver and handle transmission inhibitor.
 * Transmission can be inhibited also by asserting configured RS-232 control line, so it is
 * more convenient to combine these two things into one class
 *
 * On linux systems it is also possible to use Parallel port for the same purpose.
 */
class InhibitorAndPttControl {

	/**
	 * Set to true of serial port is configured
	 */
	bool serialConfigured;

	/**
	 * Delay after keying PTT and start of message
	 */
	int preAnouncementDelay;

	/**
	 * Set to true if transceiver is keyed now
	 */
	bool keyed;

	/**
	 * Instance of serial port library. It requires serial port to be put into constructor
	 */
	std::shared_ptr<serial::Serial> serial;

	bool serialActiveHigh;

#ifdef __linux__
	/**
	 * Set to true if parallel port is configured
	 */
	bool parallelConfigured;

	/**
	 * File descriptor for parallel port control
	 */
	int parallelPortFd;

	/**
	 * Bitmask which controls which output data pins are pulled high (+3.3V or +5V)
	 * using call to ioctl when PTT is keyed.
	 */
	uint8_t parallelPins;
#endif

public:
	InhibitorAndPttControl();
	virtual ~InhibitorAndPttControl();

	void keyPtt(bool withDelay);
	void dekeyPtt();

	/**
	 * This checks if configured serial port is accessible and if yes set the configuration
	 */
	bool setConfigAndCheckPort(std::shared_ptr<ConfigurationFile> & configurationFile);

	bool setConfigAndCheckPort(const ConfigurationFile_PttControl & configurationFile);


	static bool checkInhibitHttp(ConfigurationFile_Inhibitor & configurationFile);
	static bool checkInhibitExec(ConfigurationFile_Inhibitor & configurationFile);
	static bool checkInhibitSerial(ConfigurationFile_Inhibitor & configurationFile);



};

#endif /* INHIBITORANDPTTCONTROL_H_ */

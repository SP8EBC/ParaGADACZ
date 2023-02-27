/*
 * AvalancheWarnings.h
 *
 *  Created on: Feb 26, 2023
 *      Author: mateusz
 */

#ifndef AVALANCHEWARNINGS_H_
#define AVALANCHEWARNINGS_H_

#include <string>
#include <cstdint>

enum AvalancheWarnings_Location {
	BABIA_GORA
};

class AvalancheWarnings {

	/**
	 * URL to website with an information
	 */
	const std::string goprBabiaGora = "https://www.gopr.pl/lawiny/babia-gora";

	/**
	 * Buffer to store response from HTTP server
	 */
	std::string httpResponse;

protected:

	/**
	 *	Parse HTML from response
	 */
	void parse();

	void parse(std::string & response);

public:
	AvalancheWarnings();
	virtual ~AvalancheWarnings();

	/**
	 * Uses CURL to download warning data from TOPR/GOPR websites
	 */
	void download(AvalancheWarnings_Location location);

	/**
	 * Gets avalanche severity level from downloaded data
	 */
	int8_t getSeverityLevel();
};

#endif /* AVALANCHEWARNINGS_H_ */

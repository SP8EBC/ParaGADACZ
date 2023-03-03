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

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

enum AvalancheWarnings_Location {
	BABIA_GORA
};

class AvalancheWarnings_Expositions {
public:

	bool north;
	bool northEast;
	bool east;
	bool southEast;
	bool south;
	bool southWest;
	bool west;
	bool northWest;

	AvalancheWarnings_Expositions() :
		north(false), northEast(false), east(false), southEast(false), south(false), southWest(false), west(false), northWest(false) {}

	AvalancheWarnings_Expositions(const AvalancheWarnings_Expositions & in) {
		north = in.north;
		northEast = in.northEast;
		northWest = in.northWest;
		east = in.east;
		southEast = in.southEast;
		southWest = in.southWest;
		west = in.west;
		south = in.south;
	}

	bool hasExposition() {
		bool out = false;

		out = (north ? true : out);
		out = (northEast ? true : out);
		out = (east ? true : out);
		out = (southEast ? true : out);
		out = (south ? true : out);
		out = (southWest ? true : out);
		out = (west ? true : out);
		out = (northWest ? true : out);

		return out;
	}

	int howMany() const {
		int out = 0;

		north ? 	out++ : out;
		northEast ? out++ : out;
		east ? 		out++ : out;
		southEast ? out++ : out;
		south ? 	out++ : out;
		southWest ? out++ : out;
		west ? 		out++ : out;
		northWest ? out++ : out;

		return out;
	}
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

	/**
	 * Current severity level for forecast downloaded
	 */
	int8_t currentLevel;

	/**
	 * Response from HTTP server
	 */
	std::string responseBuffer;

	/**
	 * Write callback uset to store HTTP server response
	 */
	void writeCallback(char* data, size_t data_size);

protected:

	/**
	 * Used for unit testing only
	 */
	void parseLevel(std::string & response);

	void parseDangerousExposition(std::string & str);

	static int8_t parseSeverityLevelFromString(std::string & str);

	static AvalancheWarnings_Expositions parseExpositionsFromString(std::string & str);

	static size_t staticWriteCallback(char* get_data, size_t always_one, size_t get_data_size, void* userdata);

public:
	AvalancheWarnings();
	virtual ~AvalancheWarnings();

	/**
	 * Uses CURL to download warning data from TOPR/GOPR websites
	 */
	int download(AvalancheWarnings_Location location);

	/**
	 *	Parse HTML from response
	 */
	void parseLevel();

	void parseDangerousExposition();

	/**
	 * Gets avalanche severity level from downloaded data
	 */
	int8_t getSeverityLevel();
};

#endif /* AVALANCHEWARNINGS_H_ */

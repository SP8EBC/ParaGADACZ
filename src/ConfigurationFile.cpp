/*
 * ConfigurationFile.cpp
 *
 *  Created on: Dec 24, 2022
 *      Author: mateusz
 */

#include "ConfigurationFile.h"


#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

ConfigurationFile::ConfigurationFile(std::string fileName) {
	fn = fileName;
}

ConfigurationFile::~ConfigurationFile() {
}

ConfigurationFile::ConfigurationFile(const ConfigurationFile &other) {

}

ConfigurationFile::ConfigurationFile(ConfigurationFile &&other) {

}

ConfigurationFile& ConfigurationFile::operator=(
		const ConfigurationFile &other) {

}

bool ConfigurationFile::parse() {
	spdlog::info("parsing configuration file: {1}", this->fn);

	bool out = true;

	// read and parse configuration file
	config.readFile(this->fn.c_str());

	// get root of a configuration tree
	libconfig::Setting& root = config.getRoot();

	// get debug
	if (!root.lookupValue("Debug", this->debug)) {
		this->debug = false;
	}

	// get audio base path
	if (!root.lookupValue("AudioBasePath", this->audioBasePath)) {
		this->audioBasePath = "";
	}

	// get maximum data age
	if (!root.lookupValue("MaximumDataAge", this->maximumDataAge )) {
		this->maximumDataAge = 60;	// default value -> 60 minutes
		spdlog::warn("MaximumDataAge didn't find in configuration file! Set to default 60 minutes");
	}

	// get 'Intro' configuration section
	try {
		libconfig::Setting &intro = root["Intro"];

		intro.lookupValue("Ident", this->intro.ident);
		intro.lookupValue("SayCurrentTime", this->intro.sayCurrentTime);
		intro.lookupValue("SayRegionalPressure", this->intro.sayRegionalPressure);
	}
	catch (...) {
		spdlog::warn("Intro section didn't find in configuration file! Set to defaults");

		// set defaults values in case of exception
		this->intro.ident = "";
		this->intro.sayCurrentTime = true;
		this->intro.sayRegionalPressure = false;
	}

	// get 'RecordedSpecialAnnouncementPre'
	try {
		// get prerecorded special announcement (after ident/intro but before weather)
		libconfig::Setting &prerecordedBefore = root["RecordedSpecialAnnouncementPre"];

		// number of files to play
		size_t size = prerecordedBefore.getLength();

		if (size > 0) {
			for (int i = 0; i < size; i++) {
				std::string s = root["RecordedSpecialAnnouncementPre"][i];

				// add all files
				recordedSpecialAnnouncementPre.push_back(s);
			}
		}

		spdlog::info("{1} recorded special pre-announcement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	// get 'TextSpecialAnnouncementPre'
	try {
		// get prerecorded special announcement (after ident/intro but before weather)
		libconfig::Setting &prerecordedBefore = root["TextSpecialAnnouncementPre"];

		// number of files to play
		size_t size = prerecordedBefore.getLength();

		if (size > 0) {
			for (size_t i = 0; i < size; i++) {
				std::string s = root["TextSpecialAnnouncementPre"][i];

				// add all files
				textSpecialAnnouncementPre.push_back(s);
			}
		}

		spdlog::info("{1} text special pre=anouncement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	// current weather configuration - definition of weather stations to get data from
	try {
		libconfig::Setting & currentWeather = root["CurrentWeather"];

		int size = currentWeather.getLength();

		if (size > 0) {
			for (int i = 0; i < size; i++) {
				ConfigurationFile_CurrentWeather c;

				std::string stype;
				currentWeather[i].lookupValue("Type", stype);
				ConfigurationFile_CurrentWeatherType type = ConfigurationFile::currentWeatherTypeFromString(stype);
				c.type = type;

				currentWeather[i].lookupValue("Name", c.name);
				currentWeather[i].lookupValue("NameIdent", c.nameIdent);
				currentWeather[i].lookupValue("SayTemperature", c.sayTemperature);
				currentWeather[i].lookupValue("SayWind", c.sayWind);
				currentWeather[i].lookupValue("SayHumidity", c.sayHumidy);
				currentWeather[i].lookupValue("SayPressure", c.sayPressure);
				currentWeather[i].lookupValue("SayTemperature", c.sayTemperature);

			}
		}
		else {
			// this section is mandatory. return fail if it is empty
			out = false;
		}
	}
	catch (...) {
		out = false;
	}

	return out;
}

ConfigurationFile& ConfigurationFile::operator=(ConfigurationFile &&other) {

}


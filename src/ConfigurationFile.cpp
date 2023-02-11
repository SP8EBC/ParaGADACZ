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
	hasPogodacc = false;
	hasAprx = false;
	debug = false;
	maximumDataAge = 60;
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
	SPDLOG_INFO("parsing configuration file: {}", this->fn);

	bool out = true;

	try {
		// read and parse configuration file
		config.readFile(this->fn.c_str());
	}
	catch (libconfig::FileIOException & e) {
		SPDLOG_ERROR("libconfig::FileIOException has been trown: {}", e.what());
		return false;
	}

	// get root of a configuration tree
	libconfig::Setting& root = config.getRoot();

	// get debug
	if (!root.lookupValue("Debug", this->debug)) {
		this->debug = false;
	}

	// get audio base path
	if (!root.lookupValue("LogOutput", this->logOutput)) {
		this->logOutput = "";
	}

	// get log output
	if (!root.lookupValue("AudioBasePath", this->audioBasePath)) {
		this->audioBasePath = "";
	}

	// get maximum data age
	if (!root.lookupValue("MaximumDataAge", this->maximumDataAge )) {
		this->maximumDataAge = 60;	// default value -> 60 minutes
		SPDLOG_WARN("MaximumDataAge didn't find in configuration file! Set to default 60 minutes");
	}

	// get path to APRX rf log file
	if (!root.lookupValue("AprsLogPath", this->aprxRfLogPath )) {
		this->aprxRfLogPath = "/var/log/aprx/aprs-rf.log"; // default value -> 60 minutes
		SPDLOG_WARN("No path to APRX rf log has been provided. Using default /var/log/aprx/aprs-rf.log");
	}

	// get 'Intro' configuration section
	try {
		libconfig::Setting &intro = root["Intro"];

		intro.lookupValue("Ident", this->intro.ident);
		intro.lookupValue("SayCurrentTime", this->intro.sayCurrentTime);
		intro.lookupValue("SayRegionalPressure", this->intro.sayRegionalPressure);
	}
	catch (...) {
		SPDLOG_WARN("'Intro' section didn't find in configuration file! Set to defaults");

		// set defaults values in case of exception
		this->intro.ident = "";
		this->intro.sayCurrentTime = true;
		this->intro.sayRegionalPressure = false;
	}

	//
	// get 'RecordedSpecialAnnouncementPre'
	//
	try {
		// get prerecorded special announcement (after ident/intro but before weather)
		libconfig::Setting &prerecordedBefore = root["RecordedSpecialAnnouncementPre"];

		// number of files to play
		size_t size = prerecordedBefore.getLength();

		if (size > 0) {
			for (size_t i = 0; i < size; i++) {
				std::string s = root["RecordedSpecialAnnouncementPre"][i];

				// add all files
				recordedSpecialAnnouncementPre.push_back(s);
			}
		}

		SPDLOG_INFO("{} recorded special pre-announcement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	//
	// get 'TextSpecialAnnouncementPre'
	//
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

		SPDLOG_INFO("{} text special pre-anouncement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	//
	// 'CurrentWeather' configuration - definition of weather stations to get data from
	//
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
				currentWeather[i].lookupValue("NameIdent", c.fnIdent);
				currentWeather[i].lookupValue("SayTemperature", c.sayTemperature);
				currentWeather[i].lookupValue("SayWind", c.sayWind);
				currentWeather[i].lookupValue("SayHumidity", c.sayHumidy);
				currentWeather[i].lookupValue("SayPressure", c.sayPressure);
				currentWeather[i].lookupValue("SayTemperature", c.sayTemperature);
				currentWeather[i].lookupValue("RegionalPressure", c.regionalPressure);

				this->current.push_back(c);

				if (c.type == POGODA_CC) {
					hasPogodacc = true;
				}
				else if (c.type == APRX) {
					hasAprx = true;
				}
			}

			SPDLOG_INFO("{} sources of current weather read", size);
		}
		else {
			SPDLOG_ERROR("'CurrentWeather' section didn't find in the configuration file");
			// this section is mandatory. return fail if it is empty
			out = false;
		}
	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_ERROR("SettingNotFoundException during parsing 'CurrentWeather', e.getPath = {}", e.getPath());

		out = false;
	}
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("ParseException during parsing 'CurrentWeather', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());

		out = false;
	}

	//
	// Meteoblue forecast configuration 'ForecastMeteoblue'
	//
	try{
		libconfig::Setting & forecastMeteblue = root["ForecastMeteoblue"];

		// time from now, for which forecasts should be get
		forecastMeteblue.lookupValue("FutureTime", this->forecast.futureTime);

		// forecast could be defined but not enabled
		forecastMeteblue.lookupValue("Enable", this->forecast.enable);

		// get all locations forecast should be retrieved for
		libconfig::Setting & forecastPoints = forecastMeteblue["Locations"];

		// iterate through all points
		for (int i = 0; i < forecastPoints.getLength(); i++) {
			ConfigurationFile_ForecastMeteoblue_Locations l;

			forecastPoints[i].lookupValue("Name", l.name);
			forecastPoints[i].lookupValue("Latitude", l.latitude);
			forecastPoints[i].lookupValue("Longitude", l.longitude);
			forecastPoints[i].lookupValue("NameIdent", l.nameIdent);
			forecastPoints[i].lookupValue("SayWind", l.sayWind);
			forecastPoints[i].lookupValue("SayTemperature", l.sayTemperature);
			forecastPoints[i].lookupValue("SayPrecipation", l.sayPrecipation);

			// add this to program configuration
			this->forecast.locations.push_back(l);

		}

		SPDLOG_INFO("{} sources of meteoblue weather forecast read", forecastPoints.getLength());
	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_WARN("SettingNotFoundException during parsing 'ForecastMeteoblue', e.getPath = {}", e.getPath());

		this->forecast.enable = false;
	}
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("ParseException during parsing 'ForecastMeteoblue', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());

		out = false;
	}

	//
	// get 'RecordedSpecialAnnouncementPost'
	//
	try {
		// get prerecorded special announcement (Post -> at the end but before sign-off/outro)
		libconfig::Setting &prerecordedBefore = root["RecordedSpecialAnnouncementPost"];

		// number of files to play
		size_t size = prerecordedBefore.getLength();

		if (size > 0) {
			for (size_t i = 0; i < size; i++) {
				std::string s = root["RecordedSpecialAnnouncementPost"][i];

				// add all files
				recordedSpecialAnnouncementPost.push_back(s);
			}
		}

		SPDLOG_INFO("{1} recorded special pre-announcement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	//
	// get 'TextSpecialAnnouncementPost'
	//
	try {
		// get prerecorded special announcement (Post -> at the end but before sign-off/outro)
		libconfig::Setting &prerecordedBefore = root["TextSpecialAnnouncementPost"];

		// number of files to play
		size_t size = prerecordedBefore.getLength();

		if (size > 0) {
			for (size_t i = 0; i < size; i++) {
				std::string s = root["TextSpecialAnnouncementPost"][i];

				// add all files
				textSpecialAnnouncementPost.push_back(s);
			}
		}

		SPDLOG_INFO("{1} text special pre=anouncement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	return out;
}

ConfigurationFile& ConfigurationFile::operator=(ConfigurationFile &&other) {

}


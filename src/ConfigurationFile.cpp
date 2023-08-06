/*
 * ConfigurationFile.cpp
 *
 *  Created on: Dec 24, 2022
 *      Author: mateusz
 */

#include "ConfigurationFile.h"


#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <boost/algorithm/string.hpp>


ConfigurationFile::ConfigurationFile(std::string fileName) {
	fn = fileName;
	hasPogodacc = false;
	hasAprx = false;
	debug = false;
	maximumDataAgeInMinutes = 60;

	inhibitor.serial.enable = false;
	inhibitor.http.enable = false;
	inhibitor.exec.enable = false;

	pttControl.preAnounmntDelay = 200;
	pttControl.postAnounmntDelay = 0;

	pogodaCc.ignoreHumidityQf = false;
	pogodaCc.ignorePressureQf = false;
	pogodaCc.ignoreTemperatureQf = false;
	pogodaCc.ignoreWindQf = false;

	aprxConfig.aprxRfLogTimeInLocal = false;

	avalancheWarning.goprBabiaGora = false;

}

ConfigurationFile_CurrentWeatherType ConfigurationFile::currentWeatherTypeFromString(
		std::string in) {

			ConfigurationFile_CurrentWeatherType out = UNSET;

			boost::algorithm::to_upper(in);
			switch(swstring(in.c_str())) {
				case swstring("APRX"): out = APRX; break;
				case swstring("POGODACC"): out = POGODA_CC; break;
				case swstring("DAVIS"): out = DAVIS; break;
				case swstring("WEATHERLINK"): out = DAVIS; break;
			}

			return out;

}

std::tuple<std::string, unsigned> ConfigurationFile::splitCallsign(
		std::string input) {

			auto it = input.begin();

			std::string calsign, bufSSID;
			unsigned ssid;

			// copy callsign to separate buffer
			while ( (*it != '-') && (*it != '>') && it != input.end()) {
				calsign.append(1, *it);

				it++;
			}

			// check SSID
			if (*it == '-') {
				// copy if exist
				it++;

				while ((*it != '>') && it != input.end()) {
					bufSSID.append(1, *it);

					it++;
				}

				// and convert to integer
				ssid = std::stoi(bufSSID);
			}
			else {
				ssid = 0;
			}

			return std::make_tuple(calsign, ssid);
}

ConfigurationFile::~ConfigurationFile() {
	SPDLOG_DEBUG("");
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
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("libconfig::ParseException, line: {}, error: {}", e.getLine(), e.getError());
		return false;
	}

	// get root of a configuration tree
	libconfig::Setting& root = config.getRoot();

	// get debug
	if (!root.lookupValue("Debug", this->debug)) {
		this->debug = false;
	}

	// get log output
	if (!root.lookupValue("LogOutput", this->logOutput)) {
		this->logOutput = "/var/log/paragadacz.log";
	}

	// get audio base path
	if (!root.lookupValue("AudioBasePath", this->audioBasePath)) {
		this->audioBasePath = "/usr/share/paragadacz";
	}

	// get 'secrets'
	try {
		libconfig::Setting &secrets = root["Secrets"];

		secrets.lookupValue("MeteoblueKey", this->secrets.meteoblueKey);
		secrets.lookupValue("WeatherlinkPassword", this->secrets.weatherlinkPassword);
		secrets.lookupValue("WeatherlinkToken", this->secrets.weatherlinkToken);

	}
	catch (...) {
		SPDLOG_WARN("failure during reading API keys and tokens. some online functions may not work correctly.");
	}

	// get 'Inhibitor'
	try {
		libconfig::Setting &inhibitor = root["Inhibitor"];

		libconfig::Setting &inhibitorSerial = inhibitor["Serial"];
		libconfig::Setting &inhibitorExec = inhibitor["Exec"];
		libconfig::Setting &inhibitorHttp = inhibitor["Http"];

		inhibitorSerial.lookupValue("Enable", this->inhibitor.serial.enable);
		inhibitorSerial.lookupValue("OkActiveLevel", this->inhibitor.serial.okActiveLevel);
		inhibitorSerial.lookupValue("SerialPort", this->inhibitor.serial.port);

		inhibitorExec.lookupValue("Enable", this->inhibitor.exec.enable);
		inhibitorExec.lookupValue("OkRetval", this->inhibitor.exec.okRetval);
		inhibitorExec.lookupValue("Path", this->inhibitor.exec.path);

		inhibitorHttp.lookupValue("Enable", this->inhibitor.http.enable);
		inhibitorHttp.lookupValue("IgnoreNoAnswer", this->inhibitor.http.ignoreNoAnswer);
		inhibitorHttp.lookupValue("Url", this->inhibitor.http.url);

	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_INFO("SettingNotFoundException during parsing 'Inhibitor', inhibitor won't be enabled");

		this->inhibitor.serial.enable = false;
		this->inhibitor.http.enable = false;
		this->inhibitor.exec.enable = false;
	}
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("ParseException during parsing 'Inhibitor', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());

		out = false;
	}

	// get 'PttControl'
	try {
		libconfig::Setting &pttControl = root["PttControl"];

		pttControl.lookupValue("SerialPort", this->pttControl.pttSerialportDevice);
		pttControl.lookupValue("DelayPre", this->pttControl.preAnounmntDelay);
		pttControl.lookupValue("DelayPost", this->pttControl.postAnounmntDelay);

		// parallel port
		pttControl.lookupValue("ParallelPort", 	this->pttControl.pttParportDevice);
		pttControl.lookupValue("ParallelLines", this->pttControl.pttParportLinesSelector);


	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_ERROR("SettingNotFoundException during parsing 'PttControl', e.getPath = {}", e.getPath());

		out = false;

	}
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("ParseException during parsing 'Inhibitor', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());

		out = false;
	}

	// get maximum data age
	if (!root.lookupValue("MaximumDataAge", this->maximumDataAgeInMinutes )) {
		this->maximumDataAgeInMinutes = 60;	// default value -> 60 minutes
		SPDLOG_WARN("MaximumDataAge didn't find in configuration file! Set to default 60 minutes");
	}

	// get 'Aprx' section
	try {
		libconfig::Setting &intro = root["Aprx"];

		// base URL
		if (!intro.lookupValue("LogPath", this->aprxConfig.aprxRfLogPath)) {
			this->aprxConfig.aprxRfLogPath = "/var/log/aprx/aprs-rf.log";
		}
		intro.lookupValue("LogTimeInLocal", this->aprxConfig.aprxRfLogTimeInLocal);

	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_WARN("Configuration section for APRX log parser doesn't found. Using default values.");
		this->aprxConfig.aprxRfLogPath = "/var/log/aprx/aprs-rf.log";
		this->aprxConfig.aprxRfLogTimeInLocal = false;
	}

	// get 'Pogodacc' section
	try {
		libconfig::Setting &intro = root["Pogodacc"];

		// base URL
		if (!intro.lookupValue("BaseUrl", this->pogodaCc.baseUrl)) {
			this->pogodaCc.baseUrl = "http://pogoda.cc:8080/meteo_backend_web/";
		}

		intro.lookupValue("IgnoreTemperatureQf", this->pogodaCc.ignoreTemperatureQf);
		intro.lookupValue("IgnoreWindQf", this->pogodaCc.ignoreWindQf);
		intro.lookupValue("IgnoreHumidityQf", this->pogodaCc.ignoreHumidityQf);
		intro.lookupValue("IgnorePressureQf", this->pogodaCc.ignorePressureQf);


	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_WARN("Configuration section for pogoda.cc backend doesn't found. Using default base URL, not ignoring quality factors");
	}

//	// get maximum data age
//	if (!root.lookupValue("AprsLogInLocal", this->aprxRfLogTimeInLocal )) {
//		this->aprxRfLogTimeInLocal = false;
//		SPDLOG_INFO("Assuming that all times in APRX RF-Log are in universal time.");
//	}

	// get 'Intro' configuration section
	try {
		libconfig::Setting &intro = root["Intro"];

		// ident can be composed from few separate audio files
		libconfig::Setting &ident = intro["Ident"];

		//intro.lookupValue("Ident", this->intro.ident);
		if (ident.getLength() > 0) {
			for (int i = 0; i < ident.getLength(); i++) {
				std::string s = ident[i];

				// add all files
				this->intro.ident.push_back(s);
			}
		}

		intro.lookupValue("SayCurrentTime", this->intro.sayCurrentTime);
		intro.lookupValue("SayRegionalPressure", this->intro.sayRegionalPressure);
	}
	catch (...) {
		SPDLOG_WARN("'Intro' section didn't find in configuration file! Set to defaults");

		// set defaults values in case of exception
		//this->intro.ident = "";
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

				// don't say any local pressure if regional pressure will be sayed
				if(this->intro.sayRegionalPressure) {
					c.sayPressure = false;

					SPDLOG_WARN("Local pressure cannot be used while regional pressure is enabled!");
				}

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

		// check if mandatory configuration exists
		if (!forecastMeteblue.exists("FutureTime")) {
			SPDLOG_WARN("Couldn't find 'FutureTime' within 'ForecastMeteoblue' configuration section!");

			throw libconfig::SettingNotFoundException(forecastMeteblue, 0);
		}

		// time from now, for which forecasts should be get
		forecastMeteblue.lookupValue("FutureTime", this->forecast.futureTime);

		// forecast could be defined but not enabled
		if (!forecastMeteblue.lookupValue("Enable", this->forecast.enable)) {
			this->forecast.enable = false;
		}

		// demo mode
		if (!forecastMeteblue.lookupValue("Demo", this->forecast.demo)) {
			this->forecast.demo = false;
		}

		//
		if (!forecastMeteblue.lookupValue("SkipAnouncementIfAnyIsMissing", this->forecast.skipAnouncementIfAnyIsMissing)) {
			this->forecast.skipAnouncementIfAnyIsMissing = false;
		}

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

		// forecast cashing
		if (!forecastMeteblue.lookupValue("Cache", this->forecast.cache)) {
			this->forecast.cache = false;
		}

		// forecast cashing
		if (!forecastMeteblue.lookupValue("CacheDirectory", this->forecast.cacheDirectory)) {
			this->forecast.cacheDirectory = "/var/tmp/paragadacz";
		}

		// forecast cashing
		if (!forecastMeteblue.lookupValue("MaximumCacheAgeMins", this->forecast.cacheAgeLimit)) {
			this->forecast.cacheAgeLimit = 180;
		}

		SPDLOG_INFO("{} sources of meteoblue weather forecast read", this->forecast.locations.size());
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
	// get avalance warnings
	//
	try {
		libconfig::Setting & avalanche = root["AvalancheWarning"];

		avalanche.lookupValue("GoprBabiaGora", this->avalancheWarning.goprBabiaGora);

	}
	catch (...) {
		SPDLOG_INFO("Avalanche warning configuration not present or malformed");
		; // this section is not mandatory, so do nothing if it doesn't exist
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


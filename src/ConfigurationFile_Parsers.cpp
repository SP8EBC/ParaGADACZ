/*
 * ConfigurationFile_Parsers.cpp
 *
 *  Created on: Jan 16, 2024
 *      Author: mateusz
 */

#include "ConfigurationFile.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <boost/algorithm/string.hpp>


bool ConfigurationFile::parseInhibitor(libconfig::Setting & root) {
	bool out = true;
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
		this->inhibitor.serial.enable = false;
		this->inhibitor.http.enable = false;
		this->inhibitor.exec.enable = false;
	}
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("ParseException during parsing 'Inhibitor', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());

		out = false;
	}

	return out;

}

bool ConfigurationFile::parsePttControl(libconfig::Setting & root) {
	bool out = true;
	this->pttControl.pttSerialActiveHigh = true;

	try {
		libconfig::Setting &pttControl = root["PttControl"];

		pttControl.lookupValue("SerialPort", this->pttControl.pttSerialportDevice);
		pttControl.lookupValue("SerialActiveHigh", this->pttControl.pttSerialActiveHigh);
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

	return out;
}

bool ConfigurationFile::parseAprx(libconfig::Setting & root) {
	bool out = true;
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
	return out;
}

bool ConfigurationFile::parsePogodacc(libconfig::Setting & root) {
	bool out = true;
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

	return out;
}

bool ConfigurationFile::parseIntro(libconfig::Setting & root) {
	bool out = true;
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
		// set defaults values in case of exception
		//this->intro.ident = "";
		this->intro.sayCurrentTime = true;
		this->intro.sayRegionalPressure = false;
	}
	return out;
}

bool ConfigurationFile::parseRecordedSpecialAnnouncementPre(libconfig::Setting & root) {
	bool out = true;
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

		SPDLOG_DEBUG("{} recorded special pre-announcement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}
	return out;
}

bool ConfigurationFile::parseTextSpecialAnnouncementPre(libconfig::Setting & root) {
	bool out = true;
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

		SPDLOG_DEBUG("{} text special pre-anouncement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}
	return out;
}

bool ConfigurationFile::parseCurrentWeather(libconfig::Setting & root) {
	bool out = true;
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

				if (!currentWeather[i].lookupValue("SayTrend", c.sayTrend)) {
					c.sayTrend = false;
				}

				if (!currentWeather[i].lookupValue("DoesntSayWindDirection", c.doesntSayWinddir)) {
					c.doesntSayWinddir = false;
				}

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
				else if (c.type == DAVIS) {
					hasWeatherlink = true;
				}
			}

			SPDLOG_DEBUG("{} sources of current weather read", size);
		}
		else {
			SPDLOG_WARN("'CurrentWeather' section hasn't been found in the configuration file");
			// this section is mandatory. return fail if it is empty
		}
	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_WARN("SettingNotFoundException during parsing 'CurrentWeather', e.getPath = {}", e.getPath());
	}
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("ParseException during parsing 'CurrentWeather', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());
		out = false;

	}
	return out;
}

bool ConfigurationFile::parseTrend(libconfig::Setting & root) {
	bool out = true;
	try{
		::memset(&this->trend, 0x00, sizeof (ConfigurationFile_Trend));

		libconfig::Setting & currentWeather = root["Trend"];

		currentWeather.lookupValue("EnabledTemperature", this->trend.enabledTemperature);
		currentWeather.lookupValue("EnabledWindspeed", this->trend.enabledWindspeed);
		currentWeather.lookupValue("LongNoChangeAnouncement", this->trend.longNoChangeAnouncement);
		currentWeather.lookupValue("ShortNoChangeAnouncement", this->trend.shortNoChangeAnouncement);
		currentWeather.lookupValue("TrendLenghtInHours", this->trend.trendLenghtInHours);
		currentWeather.lookupValue("MinimumTemperatureChange", this->trend.minimumTemperatureChange);
		currentWeather.lookupValue("MinimumWindChange", this->trend.minimumWindChange);

	}
	catch (libconfig::SettingNotFoundException & e) {
		SPDLOG_WARN("SettingNotFoundException during parsing 'Trend', e.getPath = {}", e.getPath());

		this->trend.enabledTemperature = false;
		this->trend.enabledWindspeed = false;
	}
	catch (libconfig::ParseException & e) {
		SPDLOG_ERROR("ParseException during parsing 'Trend', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());

		out = false;
	}
	return out;
}

bool ConfigurationFile::parseForecastMeteoblue(libconfig::Setting & root) {
	bool out = true;
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

	return out;
}

bool ConfigurationFile::parseAvalange(libconfig::Setting & root) {
	bool out = true;
	try {
		libconfig::Setting & avalanche = root["AvalancheWarning"];

		avalanche.lookupValue("GoprBabiaGora", this->avalancheWarning.goprBabiaGora);

	}
	catch (...) {
		SPDLOG_INFO("Avalanche warning configuration not present or malformed");
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	return out;
}

bool ConfigurationFile::parseRecordedSpecialAnnouncementPost(libconfig::Setting & root) {
	bool out = true;
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

	return out;
}

bool ConfigurationFile::parseTextSpecialAnnouncementPost(libconfig::Setting & root) {
	bool out = true;
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

		SPDLOG_INFO("{1} text special preanouncement has been parsed", size);
	}
	catch (...) {
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	return out;

}

bool ConfigurationFile::parseAirspace(libconfig::Setting & root) {
	bool out = true;
	try {
		// preload default values
		this->airspace.confPerElemType.sayTRA = true;
		this->airspace.confPerElemType.sayTSA = true;
		this->airspace.confPerElemType.sayATZ = true;
		this->airspace.confPerElemType.sayD = true;
		this->airspace.confPerElemType.sayR = true;

		this->airspace.confPerElemType.atzDesignatorRegexp = "\\s[A-Z]{4}";
		this->airspace.confPerElemType.atzSectorRegexp = "[A-Z]{1}$";
		this->airspace.confPerElemType.traDesignatorRegexp = "[A-Z]{1,2}[0-9]{1,3}";
		this->airspace.confPerElemType.traSectorRegexp = "[A-Z]{1}$";
		this->airspace.confPerElemType.tsaDesignatorRegexp = "[A-Z]{1,2}[0-9]{1,3}";
		this->airspace.confPerElemType.tsaSectorRegexp = "[A-Z]{1}$";
		this->airspace.confPerElemType.dDesignatorRegexp = "[A-Z][0-9]{1,3}";
		this->airspace.confPerElemType.dSectorRegexp = "[A-Z]$";
		this->airspace.confPerElemType.rDesignatorRegexp = "[A-Z][0-9]{1,3}$";

		this->airspace.reservationFutureTimeMargin = 0;	// say everything for today
		this->airspace.sayPast = false;					// do not say any reservations which are gone
		this->airspace.sayAltitudes = false;			// no need to say from which altitudes a reservation is set
		this->airspace.genericAnouncementFromRegex = true;
		this->airspace.bailoutIfNothingToSay = false;

#define PSQL_USER 		"postgres"
#define PSQL_PASSWORD	"dupajasia123"
#define PSQL_DB			"airspace"

		this->airspace.postgresUsername = PSQL_USER;
		this->airspace.postgresPassword = PSQL_PASSWORD;
		this->airspace.postgresDb = PSQL_DB;

		libconfig::Setting &airspace = root["Airspace"];

		airspace.lookupValue("Enable", this->airspace.enabled);

		airspace.lookupValue("DumpSqlQueries", this->airspace.dumpSqlQueries);

		/*
		 * Configures point coordinates (lat, lon) with a radius in meters to look for
		 * active reservations in. If any airspace having any common part with this
		 * point+radius area has a reservation scheduled, it will be announced by
		 * separate
		 */
		if (airspace.exists("AroundPoint")) {
			libconfig::Setting &aroundPoint = airspace["AroundPoint"];
			size_t size = aroundPoint.getLength();
			SPDLOG_DEBUG("{} point-radius areas to check for active airspace reservations for", size);

			for (unsigned i = 0; i < size; i++) {
				ConfigurationFile_Airspace_AroundPoint ar;

				aroundPoint[i].lookupValue("AudioFilename", ar.audioFilename);
				aroundPoint[i].lookupValue("Latitude", ar.latitude);
				aroundPoint[i].lookupValue("Longitude", ar.longitude);
				aroundPoint[i].lookupValue("Radius", ar.radius);

				this->airspace.aroundPoint.push_back(ar);
			}
		}

		/*
		 * This configuration section defines all airspaces which reservations are checked
		 * explicitly by its designator, no matter where they are located.
		 */
		if (airspace.exists("Fixed")) {
			libconfig::Setting &fixed = airspace["Fixed"];
			size_t size = fixed.getLength();
			SPDLOG_DEBUG("{} airspaces to check for active reservations for", size);

			for (unsigned i = 0; i < size; i++) {
				ConfigurationFile_Airspace_Fixed fx;
				fx.sayAltitudes = false;
				fx.sayTimes = true;

				fixed[i].lookupValue("Designator", fx.designator);
				fixed[i].lookupValue("SayAltitudes", fx.sayAltitudes);
				fixed[i].lookupValue("SayTimes", fx.sayTimes);

				this->airspace.fixed.push_back(fx);

			}
		}

		// exit ParaGADACZ if there is no airspace reservations active
		airspace.lookupValue("BailoutIfNothingToSay", this->airspace.bailoutIfNothingToSay);

		// Say only activations which will start not later than XX minutes
		airspace.lookupValue("ReservationFutureTimeMargin", this->airspace.reservationFutureTimeMargin);

		// Say activations for today, which had expired before current time
		airspace.lookupValue("SayPast", this->airspace.sayPast);

		// Global switch to say altitude range within an airspace is reserved
		airspace.lookupValue("SayAltitudes", this->airspace.sayAltitudes);

		// Generic airspace anouncement in form of "{airspace type} {designator extracted by regexp} {sector extracted by regexp}
		airspace.lookupValue("GenericAnouncementFromRegex", this->airspace.genericAnouncementFromRegex);

		// Glue designator extracting result (if genericAnouncementFromRegex enabled) or glue full-designator-string
		airspace.lookupValue("GlueGenericAnouncement", this->airspace.glueGenericAnouncement);

		// username & password & database name use to connect to PostreSQL server with postgis and http-client extensions
		airspace.lookupValue("PostgresUsername", this->airspace.postgresUsername);
		airspace.lookupValue("PostgresPassword", this->airspace.postgresPassword);
		airspace.lookupValue("PostgresDb", this->airspace.postgresDb);

		/*
		 * Section used to specify which airspace types will be announced from the results of
		 * lookup configured by a vector of @link{ConfigurationFile_Airspace_AroundPoint}
		 * with a regular expressions used to extract designator (and sector if airspace
		 * is divided). Options sayXXX does not control anything explicitly configured
		 * by @link{ConfigurationFile_Airspace_Fixed} and those are announcement are
		 * always generated if a reservation is found.
		 *
		 * Regular expressions are used to generate data for announcements. The overall
		 * algorithm is as follows:
		 * 	1. Lookup inside 'airspaceDesignatorsAnouncement' map for explicitly configured announcement
		 * 	2. Extracting designator using regular expression. For example "EPD25A" may be
		 * 	   extracted to "D25", which will be said as (delta two five) or "D25A"
		 * 	   (said as delta two five alpha)
		 * 	3. If sector regular expression is configured, it is extracted and announced
		 * 	   separately. Final announcement for "EPD25A" might look like "delta
		 * 	   two five sector alpha"
		 * 	4. If regexp is not configured a generic announcement will be generated from
		 * 	   the full desgiantor
		 *
		 */
		if (airspace.exists("ConfigPerElemType")) {
			libconfig::Setting &perElemType = airspace["ConfigPerElemType"];

			perElemType.lookupValue("SayTRA", this->airspace.confPerElemType.sayTRA);
			perElemType.lookupValue("SayTSA", this->airspace.confPerElemType.sayTSA);
			perElemType.lookupValue("SayATZ", this->airspace.confPerElemType.sayATZ);
			perElemType.lookupValue("SayD", this->airspace.confPerElemType.sayD);
			perElemType.lookupValue("SayR", this->airspace.confPerElemType.sayR);

			perElemType.lookupValue("AtzDesignatorRegexp", this->airspace.confPerElemType.atzDesignatorRegexp);
			perElemType.lookupValue("TraDesignatorRegexp", this->airspace.confPerElemType.traDesignatorRegexp);
			perElemType.lookupValue("TraSectorRegexp", this->airspace.confPerElemType.traSectorRegexp);
			perElemType.lookupValue("TsaDesignatorRegexp", this->airspace.confPerElemType.tsaDesignatorRegexp);
			perElemType.lookupValue("TsaSectorRegexp", this->airspace.confPerElemType.tsaSectorRegexp);
			perElemType.lookupValue("DDesignatorRegexp", this->airspace.confPerElemType.dDesignatorRegexp);
			perElemType.lookupValue("DSectorRegexp", this->airspace.confPerElemType.dSectorRegexp);
			perElemType.lookupValue("RDesignatorRegexp", this->airspace.confPerElemType.rDesignatorRegexp);


		}
		else {
			;
		}

		/*
		 * This dictionary contains mapping between airspace designator and
		 * an audio file with announcement / ident. It is used by the app
		 * for any airspace reservation returned by the API. If exact match
		 * is not found within this map, the application generate a generic
		 * anonuncement used
		 */
		if (airspace.exists("AirspaceDesignatorsAnouncement")) {
			libconfig::Setting &airspaceDesignators = airspace["AirspaceDesignatorsAnouncement"];

			size_t size = airspaceDesignators.getLength();

			for (unsigned i = 0; i < size; i++) {
				std::string designator;
				airspaceDesignators[i].lookupValue("Designator", designator);

				std::string audioFilename;
				airspaceDesignators[i].lookupValue("AudioFilename", audioFilename);

				this->airspace.airspaceDesignatorsAnouncement.emplace(designator, audioFilename);
			}

		}

#ifndef PANSA_AIRSPACE_ENABLED
		if (this->airspace.enabled) {
			SPDLOG_WARN("You have enabled PANSA AUP announcements but You are using ParaGADACZ build w/o this feature");
			SPDLOG_WARN("It is really something You want to do??");
		}
#endif
	}
	catch (libconfig::SettingNotFoundException & e) {
		this->airspace.enabled = false;
	}
	catch (libconfig::ParseException & e) {
		this->airspace.enabled = false;

		SPDLOG_ERROR("ParseException during parsing 'Airspace', e.getLine = {}, e.getError = {}", e.getLine(), e.getError());

		out = false;
	}

	return out;

}

bool ConfigurationFile::parseEmailConfig(libconfig::Setting & root) {
	bool out = true;
	try {
		bool emailPop3LookupResult = true;
		bool emailImapLookupResult = true;

		emailAnnonuncements.enabled = false;
		emailAnnonuncements.bailoutIfNoCommWithServer = false;
		emailAnnonuncements.maximumLenghtInWords = 999;
		emailAnnonuncements.serverConfig.startTls = false;
		emailAnnonuncements.serverConfig.pop3Port = 0;
		emailAnnonuncements.serverConfig.imapPort = 0;
		emailAnnonuncements.serverConfig.folder = "";

		// get prerecorded special announcement (Post -> at the end but before sign-off/outro)
		libconfig::Setting &email = root["EmailSpecialAnnouncement"];
		libconfig::Setting &emailServer = email["ServerConfig"];
		libconfig::Setting &allowedSenders = email["AllowedSenders"];

		email.lookupValue("Enable", emailAnnonuncements.enabled);
		email.lookupValue("MaximumLenghtInWords", emailAnnonuncements.maximumLenghtInWords);
		email.lookupValue("BailoutIfNoCommWithServer", emailAnnonuncements.bailoutIfNoCommWithServer);

		// how many allowed senders are configured
		size_t allowedSendersSize = allowedSenders.getLength();

		// how many emails should be fetched from an inbox
		emailAnnonuncements.inboxEmailFetchLenght = 10;
		email.lookupValue("InboxEmailFetchLenght", emailAnnonuncements.inboxEmailFetchLenght);

		// at least one allowed sender must be configured
		if (allowedSendersSize == 0) {
			SPDLOG_ERROR("At least one allowed sender must be configured in EmailSpecialAnnouncement::AllowedSenders");
			throw std::runtime_error("");
		}

		// email server configuration
		emailPop3LookupResult = emailServer.lookupValue("Pop3Address", emailAnnonuncements.serverConfig.pop3Address) ? emailPop3LookupResult : false;
		emailPop3LookupResult = emailServer.lookupValue("Pop3Port", emailAnnonuncements.serverConfig.pop3Port) ? emailPop3LookupResult : false;
		emailImapLookupResult = emailServer.lookupValue("ImapAddress", emailAnnonuncements.serverConfig.imapAddress) ? emailImapLookupResult : false;
		emailImapLookupResult = emailServer.lookupValue("ImapPort", emailAnnonuncements.serverConfig.imapPort) ? emailImapLookupResult : false;

		if (!emailPop3LookupResult && !emailImapLookupResult) {
			SPDLOG_ERROR("Neighter IMAP and POP3 server is configured in EmailSpecialAnnouncement::ServerConfig");
			throw std::runtime_error("");
		}

		if (!emailServer.lookupValue("Username", emailAnnonuncements.serverConfig.username)) {
			SPDLOG_ERROR("You must specify an email address to get announcements from!");
			throw std::runtime_error("");
		}

		SPDLOG_DEBUG("Email configuration, ImapAddress: {}, ImapPort: {}, Username: {}",
						emailAnnonuncements.serverConfig.imapAddress,
						emailAnnonuncements.serverConfig.imapPort,
						emailAnnonuncements.serverConfig.username);

		emailServer.lookupValue("StartTls", emailAnnonuncements.serverConfig.startTls);
		emailServer.lookupValue("Folder", emailAnnonuncements.serverConfig.folder);

//		ConfigurationFile_Email_AllowedSender deflt;
//		deflt.emailAddress = "sp8ebc@interia.pl";
//		deflt.singleAnnouncement = false;
//		deflt.timedAnnouncement = true;
//		deflt.defaultAnnouncement = true;
//		deflt.defaultAnnouncementLn = 123;
//		emailAnnonuncements.allowedSendersList.push_back(deflt);

		for (size_t i = 0; i < allowedSendersSize; i++) {
			ConfigurationFile_Email_AllowedSender sender;

			if (!allowedSenders[i].lookupValue("EmailAddress", sender.emailAddress)) {
				SPDLOG_ERROR("Email address in EmailSpecialAnnouncement::AllowedSenders must be specified for an announcement sender. Skipping this entry.");
				continue;
			}
			allowedSenders[i].lookupValue("SingleAnnouncement", sender.singleAnnouncement);
			allowedSenders[i].lookupValue("EodAnnouncement", sender.eodAnnouncement);
			allowedSenders[i].lookupValue("TimedAnnouncement", sender.timedAnnouncement);
			allowedSenders[i].lookupValue("DefaultAnnouncement", sender.defaultAnnouncement);
			if (!allowedSenders[i].lookupValue("DefaultAnnouncementLn", sender.defaultAnnouncementLn)) {
				sender.defaultAnnouncementLn = 240;
			}

			try {
				libconfig::Setting &prep = allowedSenders[i]["Preprocessing"];
				prep.lookupValue("StartFromParagraph", sender.preprocessing.startFromParagraph);
				prep.lookupValue("EndOnParagraph", sender.preprocessing.endOnParagraph);
				prep.lookupValue("InhibitSizeLimit", sender.preprocessing.inhibitSizeLimit);

			}
			catch (...) {
				// set some default values if there is something from with content of configuration file
				sender.preprocessing.startFromParagraph = 0;
				sender.preprocessing.endOnParagraph = 0;
				sender.preprocessing.inhibitSizeLimit = false;
			}

			// add parsed sender to list
			emailAnnonuncements.allowedSendersList.push_back(sender);

			SPDLOG_DEBUG("Configuration for sender {}, single: {}, eod: {}, timed: {}",
					sender.emailAddress,
					sender.singleAnnouncement,
					sender.eodAnnouncement,
					sender.timedAnnouncement);
		}
	}
	catch (...) {
		emailAnnonuncements.enabled = false;
		SPDLOG_WARN("Email anonuncements not enabled due to error in parsing config file");
		; // this section is not mandatory, so do nothing if it doesn't exist
	}

	return out;
}

bool ConfigurationFile::parseSpeechSynthesis(libconfig::Setting & root) {
	bool out = true;
	try {
		int temp = 0;

		if (emailAnnonuncements.enabled) {

			libconfig::Setting &tts = root["SpeechSynthesis"];

			if (!tts.lookupValue("IndexFilePath", speechSynthesis.indexFilePath)) {
				SPDLOG_ERROR("Path to index file files hasn't been specified!");
				throw std::runtime_error("");
			}

			if (!tts.lookupValue("AudioFilesDirectoryPath", speechSynthesis.audioBasePath)) {
				SPDLOG_ERROR("Base directory for TTS audio files hasn't been specified!");
				throw std::runtime_error("");
			}

			if (!tts.lookupValue("TakeOnlyLastEmail", speechSynthesis.takeOnlyLastEmail)) {
				speechSynthesis.takeOnlyLastEmail = false;
			}

			if (!tts.lookupValue("BailoutIfNoMailsToSay", speechSynthesis.bailoutIfNoMailsToSay)) {
				speechSynthesis.bailoutIfNoMailsToSay = false;
			}

			if (speechSynthesis.bailoutIfNoMailsToSay) {
				SPDLOG_INFO("Program will close if there is no emails to say!");
			}

			// placeAtTheEnd
			if (!tts.lookupValue("PlaceAtTheEnd", speechSynthesis.placeAtTheEnd)) {
				speechSynthesis.placeAtTheEnd = false;
			}

			if (!tts.lookupValue("IgnoreOlderThan", speechSynthesis.ignoreOlderThan)) {
				speechSynthesis.ignoreOlderThan = 0;
			}

			if (!tts.lookupValue("Pitch", speechSynthesis.pitch)) {
				speechSynthesis.pitch = 0.5f;
			}

			if (!tts.lookupValue("Rate", speechSynthesis.rate)) {
				speechSynthesis.rate = 0.5f;
			}

			if (!tts.lookupValue("MaximumTimeout", speechSynthesis.maximumTimeout)) {
				speechSynthesis.maximumTimeout = 5.0f;
			}
			if (speechSynthesis.maximumTimeout <= 0.1f || speechSynthesis.maximumTimeout > 64.0f) {
				speechSynthesis.maximumTimeout = 5.0f;
			}

			if (!tts.lookupValue("MaximumTries", speechSynthesis.maximumTries)) {
				speechSynthesis.maximumTries = 5;
			}
			if (speechSynthesis.maximumTries < 0 || speechSynthesis.maximumTries > 16) {
				speechSynthesis.maximumTries = 4;
			}

			if (!tts.lookupValue("DelayAfterFailedTry", temp)) {
				speechSynthesis.delayAfterFailTry = 0u;
			}
			else {
				if (temp > 255 || temp < 0) {
					speechSynthesis.delayAfterFailTry = 255u;
				}
				else {
					speechSynthesis.delayAfterFailTry = temp;
				}
			}

			std::string language;
			tts.lookupValue("Language", language);
			boost::algorithm::to_upper(language);

			if (language == "POLISH") {
				speechSynthesis.language = SPEECH_POLISH;
			}
			else {
				speechSynthesis.language = SPEECH_ENGLISH;
			}

			SPDLOG_DEBUG("SpeechSynthesis, IgnoreOlderThan: {}, Pitch: {}, Rate: {}", speechSynthesis.ignoreOlderThan, speechSynthesis.pitch, speechSynthesis.rate);
			SPDLOG_DEBUG("SpeechSynthesis, MaximumTimeout: {}, MaximumTries: {}, DelayAfterFailTry: {}", speechSynthesis.maximumTimeout, speechSynthesis.maximumTries, speechSynthesis.delayAfterFailTry);
		}

	}
	catch (...) {
		emailAnnonuncements.enabled = false;
	}

	return out;
}


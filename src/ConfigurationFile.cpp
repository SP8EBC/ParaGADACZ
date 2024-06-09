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

#include <exception>


ConfigurationFile::ConfigurationFile(std::string fileName) {
	fn = fileName;
	hasPogodacc = false;
	hasAprx = false;
	hasWeatherlink = false;
	debug = false;
	onlyAssemblePlaylist = false;
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
	//SPDLOG_DEBUG("");
}

bool ConfigurationFile::parse() {
	SPDLOG_INFO("parsing configuration file: {}", this->fn);

	bool out = true;
	bool parsingResult = false;

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
	libconfig::Setting & root = config.getRoot();

	// get debug
	if (!root.lookupValue("Debug", this->debug)) {
		this->debug = false;
	}

	// get only assemble playlist
	if (!root.lookupValue("OnlyAssemblePlaylist", this->onlyAssemblePlaylist)) {
		this->onlyAssemblePlaylist = false;
	}

	if (!root.lookupValue("OnlyPttDekey", this->onlyPttDekey)) {
		this->onlyPttDekey = false;
	}

	// get log output
	if (!root.lookupValue("LogOutput", this->logOutput)) {
		this->logOutput = "/var/log/paragadacz.log";
	}

	// get log output
	if (!root.lookupValue("ZoneSpecificationFilePath", this->zoneSpecificationFilePath)) {
		this->zoneSpecificationFilePath = "/usr/local/share/paragadacz/date_time_zonespec.csv";
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
		secrets.lookupValue("ResponsiveVoiceApiKey", this->secrets.responsiveVoiceApiKey);
		secrets.lookupValue("EmailAnnouncementsPassword", this->emailAnnonuncements.serverConfig.password);


	}
	catch (...) {
		SPDLOG_WARN("failure during reading API keys and tokens. some online functions may not work correctly.");
	}

	// get 'Inhibitor'
	parsingResult = parseInhibitor(root);
	if (!parsingResult) {
		out = false;
	}

	// get 'PttControl'
	parsingResult = parsePttControl(root);
	if (!parsingResult) {
		out = false;
	}

	// get maximum data age
	if (!root.lookupValue("MaximumDataAge", this->maximumDataAgeInMinutes )) {
		this->maximumDataAgeInMinutes = 60;	// default value -> 60 minutes
		SPDLOG_WARN("MaximumDataAge didn't find in configuration file! Set to default 60 minutes");
	}

	// get 'Aprx' section
	parsingResult = parseAprx(root);
	if (!parsingResult) {
		out = false;
	}

	// get 'Pogodacc' section
	parsingResult = parsePogodacc(root);
	if (!parsingResult) {
		out = false;
	}

	// get 'Intro' configuration section
	parsingResult = parseIntro(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// get 'RecordedSpecialAnnouncementPre'
	//
	parsingResult = parseRecordedSpecialAnnouncementPre(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// get 'TextSpecialAnnouncementPre'
	//
	parsingResult = parseTextSpecialAnnouncementPre(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// 'CurrentWeather' configuration - definition of weather stations to get data from
	//
	parsingResult = parseCurrentWeather(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// 'Trend' configuration -
	//
	parsingResult = parseTrend(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// Meteoblue forecast configuration 'ForecastMeteoblue'
	//
	parsingResult = parseForecastMeteoblue(root);
	if (!parsingResult) {
		out = false;
	}


	//
	// get avalance warnings
	//
	parsingResult = parseAvalange(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// get 'RecordedSpecialAnnouncementPost'
	//
	parsingResult = parseRecordedSpecialAnnouncementPost(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// get 'TextSpecialAnnouncementPost'
	//
	parsingResult = parseTextSpecialAnnouncementPost(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// get 'Airspace'
	//
	parsingResult = parseAirspace(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// get 'EmailConfig'
	//
	parsingResult = parseEmailConfig(root);
	if (!parsingResult) {
		out = false;
	}

	//
	// get 'SpeechSynthesis'
	//
	parsingResult = parseSpeechSynthesis(root);
	if (!parsingResult) {
		out = false;
	}

	// get outro
	root.lookupValue("Outro", this->outro);

	return out;
}


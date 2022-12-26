/*
 * ConfigurationFile.h
 *
 *  Created on: Dec 24, 2022
 *      Author: mateusz
 */

#ifndef CONFIGURATIONFILE_H_
#define CONFIGURATIONFILE_H_

#include <string>
#include <vector>
#include <cstdint>

#include <boost/algorithm/string.hpp>

#include <libconfig.h++>

/**
 * Enum with values of field 'Type' in 'CurrentWeather'
 */
enum ConfigurationFile_CurrentWeatherType {
	APRX,
	POGODA_CC,
	HOLFUY,
	UNSET
};

/**
 * Intro section
 *
 *
 * Intro = {
 *   Ident = "ident.mp3";
 *   SayCurrentTime = false;
 *   SayRegionalPressure = true;
 * }
 *
 */
struct ConfigurationFile_Intro {
	std::string ident;		//!< Name of audio file with station ident, played at the beginning
	bool sayCurrentTime;	//!< Say current time after ident
	bool sayRegionalPressure;	//!<
};

/**
 * Current weather section, which describes all sources of
 * measurements to speak.
 */
struct ConfigurationFile_CurrentWeather {
	ConfigurationFile_CurrentWeatherType type;	//!< Source type
	std::string	name;	//!< Weather station name or its callsign in case of APRX source
	std::string nameIdent;	//<! Name of audio file with station announcement.
	bool sayTemperature;	//<! Say temperature from that station
	bool sayWind;		//<! Say wind speed, direction and gusts
	bool sayHumidy;		//<! Say humidity for that station
	bool regionalPressure;	//<! Use that station as regional pressure source, only one station can have set this
	bool sayPressure;	//<! Say pressure for that station, IF regionalPressure is not enabled
};

struct ConfigurationFile_ForecastMeteoblue_Locations {
	float latitude;
	float longitude;
	std::string nameIdent; //!< Name of audio file with this forecast point name/identification
	bool sayWind;	//!< Say wind for this forecast point
	bool sayTemperature;	//!< Say temperature for this forecast
	bool sayPrecipation;	//!< Say precipation (size and probability) for that forecast
};

struct ConfigurationFile_ForecastMeteoblue {
	uint16_t futureTime;			//!< number of minutes from now to get forecast for
	std::vector<ConfigurationFile_ForecastMeteoblue_Locations> locations;	//!< forecast loca
};

class ConfigurationFile {

	libconfig::Config config;	//!< parsed configuration and its parser

	constexpr static int swstring(const char* _in) {
		int out = 0;

		for (int i = 0; *(_in + i) != '\0'; i++) {
			out += (int)*(_in + i);
		}

		return out;
	}

	std::string fn;				//!< Path to filename with input configuration

	bool debug;

	std::string audioBasePath;	//!< Base path do directory where all audio files are put
	std::string logOutput;		//!< Path to created log file

	int maximumDataAge;	//!< maximum time in minutes for current measurements to be usable

	std::vector<std::string> recordedSpecialAnnouncementPre;
	std::vector<std::string> textSpecialAnnouncementPre;

	ConfigurationFile_Intro intro;

	ConfigurationFile_CurrentWeather current;

	ConfigurationFile_ForecastMeteoblue forecast;

	std::vector<std::string> recordedSpecialAnnouncementPost;
	std::vector<std::string> textSpecialAnnouncementPost;

public:

	static ConfigurationFile_CurrentWeatherType currentWeatherTypeFromString(std::string in) {

		ConfigurationFile_CurrentWeatherType out = UNSET;

		boost::algorithm::to_upper(in);
		switch(swstring(in.c_str())) {
			case swstring("APRX"): out = APRX; break;
			case swstring("POGODACC"): out = POGODA_CC; break;
		}

		return out;
	}

	bool parse();

	ConfigurationFile(std::string fileName);
	virtual ~ConfigurationFile();
	ConfigurationFile(const ConfigurationFile &other);
	ConfigurationFile(ConfigurationFile &&other);
	ConfigurationFile& operator=(const ConfigurationFile &other);
	ConfigurationFile& operator=(ConfigurationFile &&other);
};

#endif /* CONFIGURATIONFILE_H_ */

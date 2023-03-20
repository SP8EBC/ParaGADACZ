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
#include <tuple>

#include <libconfig.h++>

/**
 * Enum with values of field 'Type' in 'CurrentWeather'
 */
enum ConfigurationFile_CurrentWeatherType {
	APRX,
	POGODA_CC,
	HOLFUY,
	DAVIS,
	UNSET
};

struct ConfigurationFile_Secret {
	std::string meteoblueKey;
	std::string weatherlinkPassword;
	std::string weatherlinkToken;
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
	std::vector<std::string> ident;		//!< Name of audio file with station ident, played at the beginning
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
	std::string fnIdent;	//<! Name of audio file with station announcement.
	bool sayTemperature;	//<! Say temperature from that station
	bool sayWind;		//<! Say wind speed, direction and gusts
	bool sayHumidy;		//<! Say humidity for that station
	bool regionalPressure;	//<! Use that station as regional pressure source, only one station can have set this
	bool sayPressure;	//<! Say pressure for that station, IF regionalPressure is not enabled
};

struct ConfigurationFile_ForecastMeteoblue_Locations {
	std::string name;	//!< Name od this forecast point
	float latitude;
	float longitude;
	std::string nameIdent; //!< Name of audio file with this forecast point name/identification
	bool sayWind;	//!< Say wind for this forecast point
	bool sayTemperature;	//!< Say temperature for this forecast
	bool sayPrecipation;	//!< Say precipation (size and probability) for that forecast
};

struct ConfigurationFile_ForecastMeteoblue {
	bool enable;					//!< If meteo blue forecast is enabled
	uint32_t futureTime;			//!< number of minutes from now to get forecast for
	bool skipAnouncementIfAnyIsMissing;	//!< skip missing forecast announcement instead of skipping whole forecast anouncement
	bool demo;						//!< Create forecast stub FOR DEMO ONLY if API key is missing
	std::vector<ConfigurationFile_ForecastMeteoblue_Locations> locations;	//!< forecast loca
};

struct ConfigurationFile_Inhibitor_Serial {
	bool enable;
	std::string port;		//!< Which serial port should be used by inhibitor
	bool okActiveLevel;		//!< Which state on CTS line allows transmission.
};

struct ConfigurationFile_Inhibitor_Exec {
	bool enable;
	std::string path;		//!< Path to executable
	int okRetval;			//!< Which return value is OK and allows
};

struct ConfigurationFile_Inhibitor_Http {
	bool enable;
	std::string url;		//!< URL to send request to
	bool ignoreNoAnswer;	//!< Set to true to allow transmission if there is no answer from HTTP server
};

struct ConfigurationFile_Inhibitor {
	ConfigurationFile_Inhibitor_Serial serial;
	ConfigurationFile_Inhibitor_Exec exec;
	ConfigurationFile_Inhibitor_Http http;
};

struct ConfigurationFile_PttControl {
	std::string port;			// !< Port which will be used to control PTT line
	uint32_t preDelay;			// !< Delay in miliseconds after keying PTT (pulling RTS down) and playback start
	uint32_t postDelay;			// !< Delay after the end of anouncement and dekeying a transceiver
};

struct ConfigurationFile_Avalanche {
	bool goprBabiaGora;
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

	ConfigurationFile_Secret secrets;

	ConfigurationFile_Inhibitor inhibitor;
	ConfigurationFile_PttControl pttControl;

	int maximumDataAge;	//!< maximum time in minutes for current measurements to be usable
	std::string aprxRfLogPath;	//!< Path to

	std::vector<std::string> recordedSpecialAnnouncementPre;
	std::vector<std::string> textSpecialAnnouncementPre;

	ConfigurationFile_Intro intro;

	std::vector<ConfigurationFile_CurrentWeather> current;

	ConfigurationFile_ForecastMeteoblue forecast;

	ConfigurationFile_Avalanche	avalancheWarning;

	std::vector<std::string> recordedSpecialAnnouncementPost;
	std::vector<std::string> textSpecialAnnouncementPost;

	bool hasPogodacc;
	bool hasAprx;

public:

	static ConfigurationFile_CurrentWeatherType currentWeatherTypeFromString(std::string in);

	/**
	 * Splits callsign and ssid stored in one string to separate elements
	 */
	static std::tuple<std::string, unsigned> splitCallsign(std::string input);

	/**
	 * Parse configuration file from path provided to constructor.
	 */
	bool parse();

	ConfigurationFile(std::string fileName);
	virtual ~ConfigurationFile();

	const std::string& getAudioBasePath() const {
		return audioBasePath;
	}

	const std::vector<ConfigurationFile_CurrentWeather>& getCurrent() const {
		return current;
	}

	bool isDebug() const {
		return debug;
	}

	const ConfigurationFile_ForecastMeteoblue& getForecast() const {
		return forecast;
	}

	const ConfigurationFile_Intro& getIntro() const {
		return intro;
	}

	const std::string& getLogOutput() const {
		return logOutput;
	}

	int getMaximumDataAge() const {
		return maximumDataAge;
	}

	const std::vector<std::string>& getRecordedSpecialAnnouncementPost() const {
		return recordedSpecialAnnouncementPost;
	}

	const std::vector<std::string>& getRecordedSpecialAnnouncementPre() const {
		return recordedSpecialAnnouncementPre;
	}

	const std::vector<std::string>& getTextSpecialAnnouncementPost() const {
		return textSpecialAnnouncementPost;
	}

	const std::vector<std::string>& getTextSpecialAnnouncementPre() const {
		return textSpecialAnnouncementPre;
	}

	bool isHasPogodacc() const {
		return hasPogodacc;
	}

	const std::string& getAprxRfLogPath() const {
		return aprxRfLogPath;
	}

	bool isHasAprx() const {
		return hasAprx;
	}

	const ConfigurationFile_PttControl& getPttControl() const {
		return pttControl;
	}

	const ConfigurationFile_Inhibitor& getInhibitor() const {
		return inhibitor;
	}

	ConfigurationFile_Avalanche getAvalancheWarning() const {
		return avalancheWarning;
	}

	const ConfigurationFile_Secret& getSecrets() const {
		return secrets;
	}
};

#endif /* CONFIGURATIONFILE_H_ */

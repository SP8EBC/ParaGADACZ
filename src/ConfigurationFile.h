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

enum ConfigurationFile_Language {
	SPEECH_POLISH,
	SPEECH_ENGLISH
};

/**
 * APRX rf-log parser configuration
 */
struct ConfigurationFile_Aprx {
	bool aprxRfLogTimeInLocal;	//!< Set to true if timestamps in APRX log file are in local time not UTC
	std::string aprxRfLogPath;	//!< Path to
};

struct ConfigurationFile_Pogodacc {
	std::string baseUrl;			// API base url
	bool ignoreTemperatureQf;		// ignore temeprature quality factor and say it in any cases
	bool ignoreWindQf;
	bool ignoreHumidityQf;
	bool ignorePressureQf;
};

/**
 * All secrets like API keys, password and simmilar stuff which shall not be
 * shared publicity and usually is separated to separate config file,
 * which then is included into main one
 */
struct ConfigurationFile_Secret {
	std::string meteoblueKey;
	std::string weatherlinkPassword;
	std::string weatherlinkToken;
	std::string responsiveVoiceApiKey;
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
	std::string	name;		//!< Weather station name or its callsign in case of APRX source
	std::string fnIdent;	//!< Name of audio file with station announcement.
	bool sayTemperature;	//!< Say temperature from that station
	bool sayWind;			//!< Say wind speed, direction and gusts
	bool doesntSayWinddir;	//!< Set to true if wind direction shall not be said
	bool sayHumidy;			//!< Say humidity for that station
	bool regionalPressure;	//!< Use that station as regional pressure source, only one station can have set this
	bool sayPressure;		//!< Say pressure for that station, IF regionalPressure is not enabled
	 bool sayTrend;			//!< If trend shall be say for this place
};

/**
 * Additional trend announcement for all current weather configuration
 */
struct ConfigurationFile_Trend {
	int trendLenghtInHours;
	bool enabledTemperature;
	bool enabledWindspeed;
	int minimumTemperatureChange;	//!< Minimum percent change of temperature to treat as significant change;
	int minimumWindChange;			//!< Minimum percent change of wind speed to treat as significant change;
	bool shortNoChangeAnouncement;	//!< Just say "no change" if there is no change in trend
	bool longNoChangeAnouncement;	//!< Say "no chagne in last XX hours", this option has a priority
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
	bool cache;				//!< Enable forecast cache to save money on API calls
	std::string cacheDirectory;	//!< Path to directory with cache files
	int cacheAgeLimit;		//!< How old the cache might be before it is discarded and new forecast is downloaded
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
	std::string pttSerialportDevice;	// !< Port which will be used to control PTT line
	uint32_t preAnounmntDelay;			// !< Delay in miliseconds after keying PTT (pulling RTS down) and playback start
	uint32_t postAnounmntDelay;			// !< Delay after the end of anouncement and dekeying a transceiver

#ifdef __linux__
	uint32_t pttParportLinesSelector;	// !< This is a bitmask which selects which parallel port lines are used
	std::string pttParportDevice;		// !< String to device entry in /dev directory representing parallel port
#endif
};

struct ConfigurationFile_Avalanche {
	bool goprBabiaGora;
};

/**
 * Configuration of a server emails will be downloaded from
 */
struct ConfigurationFile_Email_Server {
	std::string pop3Address;	//!< POP3 server address
	int pop3Port;				//!< TCP port to use for POP3 connection
	std::string imapAddress;	//!< IMAP server address
	int imapPort;				//!< TCP post to use for IMAP connection
	std::string username;		//!< Username aka e-mail address
	std::string password;		//!< Password for this e-mail address
	bool startTls;				//!< Set to false to select auth_method_t::LOGIN, true for auth_method_t::START_TLS
};

/**
 * How an email with announcement should be preprocessed before converting
 */
struct ConfigurationFile_Email_AllowedSender_Preprocess {
	// paragraph is count as a block of text separated with more
	// than one newline
	int startFromParagraph;	//!< First paragraph to be converted text to speech
	int endOnParagraph;		//!< Last paragraph to be converted
	bool inhibitSizeLimit;	//!< This sender should obey global announcement lenght limit
};


/**
 *	Element of a list of senders allowed to send an email with anouncement, which
 *	will be converter from text to speech
 */
struct ConfigurationFile_Email_AllowedSender {
	std::string emailAddress;	//!< Email address which is allowed to be a source
	bool singleAnnouncement;	//!< if this sender can send single announcement
	bool eodAnnouncement;		//!< If this sender can send announcement for whole day
	bool timedAnnouncement;		//!< If this sender can send timed announcement, which will
	bool defaultAnnouncement;	//!< If emails with non-specified subject should be allowed
	int defaultAnnouncementLn;	//!< How long (in minutes) default announcement is valid since
	ConfigurationFile_Email_AllowedSender_Preprocess preprocessing;
};


/**
 * Email text to speech announcements configuration
 */
struct ConfigurationFile_Email {
	bool enabled;
	ConfigurationFile_Email_Server serverConfig;
	std::vector<ConfigurationFile_Email_AllowedSender> allowedSendersList;
	int maximumLenghtInWords;	//!< Maximum size of one announcement in words
};

/**
 * Configuration of SpeechSynthesis, including
 */
struct ConfigurationFile_SpeechSynthesis {
	std::string indexFilePath;	//!< Path to JSON file with an index
	int ignoreOlderThan;		//!< Automatically ignore email messages older than xx minutes
	ConfigurationFile_Language language;	//!< Language to use for TSS conversion
	float pitch;
	float rate;
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
	bool onlyAssemblePlaylist;	//!< Only for debug/development. Assembles playlist of an anoyuncement w/o playing it

	std::string audioBasePath;	//!< Base path do directory where all audio files are put
	std::string logOutput;		//!< Path to created log file

	ConfigurationFile_Secret secrets;

	ConfigurationFile_Inhibitor inhibitor;
	ConfigurationFile_PttControl pttControl;

	ConfigurationFile_Aprx aprxConfig;

	ConfigurationFile_Pogodacc pogodaCc;

	int maximumDataAgeInMinutes;	//!< maximum time in minutes for current measurements to be usable


	std::vector<std::string> recordedSpecialAnnouncementPre;
	std::vector<std::string> textSpecialAnnouncementPre;

	ConfigurationFile_Intro intro;

	std::vector<ConfigurationFile_CurrentWeather> current;
	ConfigurationFile_Trend trend;

	ConfigurationFile_ForecastMeteoblue forecast;

	ConfigurationFile_Avalanche	avalancheWarning;

	ConfigurationFile_Email	emailAnnonuncements;
	ConfigurationFile_SpeechSynthesis speechSynthesis;

	std::vector<std::string> recordedSpecialAnnouncementPost;
	std::vector<std::string> textSpecialAnnouncementPost;

	bool hasPogodacc;
	bool hasAprx;
	bool hasWeatherlink;

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

	int getMaximumDataAgeInMinutes() const {
		return maximumDataAgeInMinutes;
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

	const ConfigurationFile_Aprx& getAprxConfig() const {
		return aprxConfig;
	}

	const ConfigurationFile_Pogodacc& getPogodaCc() const {
		return pogodaCc;
	}

	bool isOnlyAssemblePlaylist() const {
		return onlyAssemblePlaylist;
	}

	const ConfigurationFile_Trend& getTrend() const {
		return trend;
	}

	const ConfigurationFile_Email& getEmailAnnonuncements() const {
		return emailAnnonuncements;
	}

	const ConfigurationFile_SpeechSynthesis& getSpeechSynthesis() const {
		return speechSynthesis;
	}

	bool isHasWeatherlink() const {
		return hasWeatherlink;
	}
};

#endif /* CONFIGURATIONFILE_H_ */

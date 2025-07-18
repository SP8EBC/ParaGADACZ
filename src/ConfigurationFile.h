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
#include <map>

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

	bool pttSerialActiveHigh;				// !< set true if PTT is active high
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
	std::string folder;
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
	bool onlyFromAttachement;	//!< If sender is allowed to only send a message with attachement content
	ConfigurationFile_Email_AllowedSender_Preprocess preprocessing;
};

/**
 * Configures point coordinates (lat, lon) with a radius in meters to look for
 * active reservations in. If any airspace having any common part with this
 * point+radius area has a reservation scheduled, it will be announced by
 * separate
 */
struct ConfigurationFile_Airspace_AroundPoint {
	std::string audioFilename;	//!< Filename used as an ident of this point
	int radius;					//!< Radius around the point to look for reservations for
	float longitude;			//!<
	float latitude;				//!<
	float maximumLowerAltitude; //!< Upper limit of an altitude to take into consideration
};

/**
 * This configuration section defines all airspaces which reservations are checked
 * explicitly by its designator, no matter where they are located.
 */
struct ConfigurationFile_Airspace_Fixed {
	std::string designator;		//!< Airspace designator as returned by the API
	/**
	 * If an altitude range shall be spoken for this airspace. False value has
	 * a priority over value of 'sayAltitudes' in @link{ConfigurationFile_Airspace}
	 * If it is disabled here, the altitude range won't be added to announcement
	 * even if @link{ConfigurationFile_Airspace} enabled that. False value of
	 * global switch has also a priority over this field, so long story short
	 *
	 *  will be spoken if:
	 *  ConfigurationFile_Airspace = true & ConfigurationFile_Airspace_Fixed = true
	 *
	 *  will not be spoken if:
	 *  ConfigurationFile_Airspace = true & ConfigurationFile_Airspace_Fixed = false
	 *  ConfigurationFile_Airspace = false & ConfigurationFile_Airspace_Fixed = true
	 *
	 *  this field is initialized with value of false if not present in config file
	 */
	bool sayAltitudes;
	bool sayTimes;
};

/**
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
 * 	   the full desgiantor aka full-designator-string
 *
 */
struct ConfigurationFile_Airspace_SayConfigPerElemType {
	bool sayATZ;	//!< If any ATZ configured by @link{ConfigurationFile_Airspace_AroundPoint} will be said
	bool sayTRA;	//!< If any TRA configured by @link{ConfigurationFile_Airspace_AroundPoint} will be said
	bool sayTSA;	//!< If any TSA configured by @link{ConfigurationFile_Airspace_AroundPoint} will be said
	bool sayD;	//!< If any D configured by @link{ConfigurationFile_Airspace_AroundPoint} will be said
	bool sayR;	//!< If any R configured by @link{ConfigurationFile_Airspace_AroundPoint} will be said
	bool sayP;	//!< If any P configured by @link{ConfigurationFile_Airspace_AroundPoint} will be said

	std::string atzDesignatorRegexp;	//!< Regexp used to extract ATZ designator to say
	std::string atzSectorRegexp;		//!< Regexp used to extract ATZ sector to say
	std::string traDesignatorRegexp;	//!< Regexp used to extract TRA designator to say
	std::string traSectorRegexp;		//!< Regexp used to extract TRA sector to say
	std::string tsaDesignatorRegexp;	//!< Regexp used to extract TSA designator to say
	std::string tsaSectorRegexp;		//!< Regexp used to extract TSA sector to say
	std::string dDesignatorRegexp;		//!< Regexp used to extract TRA designator to say
	std::string dSectorRegexp;			//!< Regexp used to extract TRA designator to say
	std::string rDesignatorRegexp;		//!< Regexp used to extract TRA designator to say
	std::string pDesignatorRegexp;		//!< Regexp used to extract TRA designator to say
};

struct ConfigurationFile_Airspace {
	bool enabled;
	bool dumpSqlQueries;
	std::string postgresUsername;	//!< Username to PostgreSQL with postgis and http-client addons
	std::string postgresPassword;	//!< Password to PostgreSQL with postgis and http-client addons
	std::string postgresDb;		//!< PostgreSQL database to use
	std::vector<ConfigurationFile_Airspace_AroundPoint> aroundPoint;
	std::vector<ConfigurationFile_Airspace_Fixed> fixed;
	int reservationFutureTimeMargin;	//!< Say only activations which will start not later than XX minutes
	bool sayPast;		//!< Say activations for today, which had expired before current time
	bool sayAltitudes;	//!< Global switch to say altitude range within an airspace is reserved
	bool genericAnouncementFromRegex;	//!< Use regular expressions to extract designator and sector for announcement
	bool glueGenericAnouncement; //!< Glue designator extracting result (if genericAnouncementFromRegex enabled) or glue full-designator-string
	bool bailoutIfNothingToSay;	//!< Exit the app if there is no active reservations in the API
	ConfigurationFile_Airspace_SayConfigPerElemType confPerElemType; //!< How each airspace type is announced
	std::vector<std::string> designatorsFilter;

	/**
	 * This dictionary contains mapping between airspace designator and
	 * an audio file with announcement / ident. It is used by the app
	 * for any airspace reservation returned by the API. If exact match
	 * is not found within this map, the application generate a generic
	 * anonuncement used
	 */
	std::map<std::string, std::string> airspaceDesignatorsAnouncement;
};

/**
 * Email text to speech announcements configuration
 */
struct ConfigurationFile_Email {
	bool enabled;
	ConfigurationFile_Email_Server serverConfig;
	std::vector<ConfigurationFile_Email_AllowedSender> allowedSendersList;
	int maximumLenghtInWords;	//!< Maximum size of one announcement in words
	int inboxEmailFetchLenght;	//!< How many emails should be downloaded from an inbox
	bool bailoutIfNoCommWithServer;//!< Abort program execution if there is no communication
};

/**
 * Configuration of SpeechSynthesis, including
 */
struct ConfigurationFile_SpeechSynthesis {
	std::string indexFilePath;	//!< Path to JSON file with an index
	std::string audioBasePath;	//!<
	int ignoreOlderThan;		//!< Automatically ignore email messages older than xx minutes
	bool takeOnlyLastEmail;		//!<
	float maximumTimeout;		//!< Maximum timeout while converting from text to speed
	int maximumTries;			//!< How many times the application will try to convert text to speech
	uint8_t delayAfterFailTry;	//!< Delay in seconds between failed TTS conversion and next try
	ConfigurationFile_Language language;	//!< Language to use for TSS conversion
	float pitch;
	float rate;
	bool bailoutIfNoMailsToSay;	//!< Abort program execution if there is nothing new to say
	bool placeAtTheEnd;			//!< Set to true to append anouncements at the end, just before outro
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
	bool onlyPttDekey;

	std::string audioBasePath;	//!< Base path do directory where all audio files are put
	std::string logOutput;		//!< Path to created log file

	std::string zoneSpecificationFilePath;	//!< Path to 'date_time_zonespec.csv' including filename itself

	bool logicalAndOnBailout;				//!< Bailout conditions for

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

	ConfigurationFile_Airspace airspace;

	ConfigurationFile_Email	emailAnnonuncements;
	ConfigurationFile_SpeechSynthesis speechSynthesis;

	std::vector<std::string> recordedSpecialAnnouncementPost;
	std::vector<std::string> textSpecialAnnouncementPost;

	std::string outro;

	bool hasPogodacc;
	bool hasAprx;
	bool hasWeatherlink;

protected:

	bool parseInhibitor(libconfig::Setting & root);
	bool parsePttControl(libconfig::Setting & root);
	bool parseAprx(libconfig::Setting & root);
	bool parsePogodacc(libconfig::Setting & root);
	bool parseIntro(libconfig::Setting & root);
	bool parseRecordedSpecialAnnouncementPre(libconfig::Setting & root);
	bool parseTextSpecialAnnouncementPre(libconfig::Setting & root);
	bool parseCurrentWeather(libconfig::Setting & root);
	bool parseTrend(libconfig::Setting & root);
	bool parseForecastMeteoblue(libconfig::Setting & root);
	bool parseAvalange(libconfig::Setting & root);
	bool parseRecordedSpecialAnnouncementPost(libconfig::Setting & root);
	bool parseTextSpecialAnnouncementPost(libconfig::Setting & root);
	bool parseAirspace(libconfig::Setting & root);
	bool parseEmailConfig(libconfig::Setting & root);
	bool parseSpeechSynthesis(libconfig::Setting & root);

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

	const std::string& getZoneSpecificationFilePath() const {
		return zoneSpecificationFilePath;
	}

	const std::string& getOutro() const {
		return outro;
	}

	const ConfigurationFile_Airspace& getAirspace() const {
		return airspace;
	}

	bool isOnlyPttDekey() const {
		return onlyPttDekey;
	}

	bool isLogicalAndOnBailout() const {
		return logicalAndOnBailout;
	}
};

#endif /* CONFIGURATIONFILE_H_ */

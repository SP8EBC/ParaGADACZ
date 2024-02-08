/*
 * PlaylistSampler.h
 *
 * This is an interface which shall be implemented by natural language specific implementation
 * used to get
 *
 *  Created on: Dec 30, 2022
 *      Author: mateusz
 */

#ifndef PLAYLISTSAMPLER_H_
#define PLAYLISTSAMPLER_H_

#ifdef PANSA_AIRSPACE_ENABLED

#endif

#include <vector>
#include <string>
#include <optional>
#include <tuple>

#include "AvalancheWarnings.h"
#ifdef PANSA_AIRSPACE_ENABLED
#include "PansaAirspaceTypes.h"
#endif

#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o

/**
 * Enumeration representing different measurement units.
 */
enum PlaylistSampler_Unit {
	KNOTS,
	MS,		//!< Meters per second
	DEG,	//!< Degrees (like for wind direction)
	PERCENT,
	CELSIUS,//!< Degrees Celsius
	HPA,	//!<
	MILIMETER,
	METER,
	KM,		//!< Kilometers
	NM		//!< Nautical miles
};

/**
 * Constant element of an anouncement.
 */
enum PlaylistSampler_ConstanElement {
	WEATHER,		//!< "weather"
	CURRENT_TIME,	//!< like "Current time is"
	CURRENT_WEATHER,//!< "Current weather conditions"
	FORECAST,		//!< "Forecast for next"
	NO_CHANGE,
	DURING,
	INCREASE,
	DROP,
	QNH,
	REGIONAL_QNH,	//!< "Regional pressure"
	HOUR,			//!< "hour" (one)
	HOURS,			//!< "hours"
	WIND,			//!< "wind"
	WIND_GUSTS,		//!< "gusts"
	TEMPERATURE,	//!< "temperature"
	HUMIDITY,		//!< "humidity
	NO_PRECIPATION,
	PRECIPATION,	//!< "precipation"
	INTERMITTENT,	//!<
	RAIN,
	SNOW,
	THUNDERSTORM,
	FROM,
	UO_TO,			//!< "up to xxx"
	ABOVE,			//!< "more than xxx" / "above xxx"
	INTENSE,
	LOCAL,			//!< "local"
	POSSIBLE,		//!< "possibility" of something
	AVALANCHE_WARNING,
	DANGEROUS_EXPOSITION,
	FIRST_LEVEL,	//!< avalanche warning level
	SECOND_LEVEL,	//!< avalanche warning level
	THIRD_LEVEL,	//!< avalanche warning level
	FOURTH_LEVEL,	//!< avalanche warning level
	ALSO,
	SPECIAL_ANOUNCEMENT,
	REGION,			//!< "region"
	RADIUS,			//!< "radius"
	IN_RADIUS,		//!< "in radius of"
	AROUND,			//!< "around"
	FROM_LOCATION	//!< "from location"
};

enum PlaylistSampler_Phonetic {
	PH_ZERO			= 0x30,
	PH_ONE			= 0x31,
	PH_TWO			= 0x32,
	PH_THREE		= 0x33,
	PH_FOUR			= 0x34,
	PH_FIVE			= 0x35,
	PH_SIX			= 0x36,
	PH_SEVEN		= 0x37,
	PH_EIGHT		= 0x38,
	PH_NINE			= 0x39,
	PH_ALPHA		= 0x41,
	PH_BRAVO		= 0x42,
	PH_CHARLIE		= 0x43,
	PH_DELTA		= 0x44,
	PH_ECHO			= 0x45,
	PH_FOXTROT		= 0x46,
	PH_GOLF			= 0x47,
	PH_HOTEL		= 0x48,
	PH_INDIA		= 0x49,
	PH_JULIET		= 0x4A,
	PH_KILO			= 0x4B,
	PH_LIMA			= 0x4C,
	PH_MIKE			= 0x4D,
	PH_NOVEMBER		= 0x4E,
	PH_OSCAR		= 0x4F,
	PH_PAPA			= 0x50,
	PH_QUEBEC		= 0x51,
	PH_ROMEO		= 0x52,
	PH_SIERRA		= 0x53,
	PH_TANGO		= 0x54,
	PH_UNIFORM		= 0x55,
	PH_VICTOR		= 0x56,
	PH_WISKEY		= 0x57,
	PH_XRAY			= 0x58,
	PH_YANKEE		= 0x59,
	PH_ZULU			= 0x5A
};

enum PlaylistSampler_Airspace {
	AIRSPACE_RESTRICTIONS_IN,	//!< "Flight restrictions in area"
	AIRSPACE_ZONE,				//!< "Zone"
	AIRSPACE_SECTOR,
	AIRSPACE_ALTITUDE,
	AIRSPACE_GROUND,
	AIRSPACE_ACTIVE
};

class PlaylistSampler {

	std::string locale;

public:

	/**
	 * Returns a list of audio files representing given integer number (may be negative).
	 * As an example, number -123 will return in English:
	 * 		minus.mp3
	 * 		one.mp3
	 *		hundret.mp3
	 *		twenty.mp3
	 *		three.mp3
	 *
	 *	\param integer Number to convert to audio files
	 *	\return Vector of paths to audio files representing number
	 */
	virtual std::vector<std::string> getAudioListFromNumber(int integer) = 0;

	/**
	 * Returns a list of audio files representing given floating point number.
	 * Method always reduce precision to one digit after decimal point
	 * As an example, number -1.45 will return in English:
	 * 		minus.mp3
	 * 		one.mp3
	 * 		decimal.mp3
	 * 		four.mp3
	 *
	 * \param decimal Number to convert to audio files
	 * \return Vector of paths to audio files representing number
	 *
	 */
	virtual std::vector<std::string> getAudioListFromNumber(float decimal) = 0;

	/**
	 * Returns single path to audio file
	 *
	 * \param unit Measurement unit to get audio file for
	 * \param value Needed for language with which has declination depending on number
	 * \return Path to audio file
	 */
	virtual std::string getAudioFromUnit(PlaylistSampler_Unit unit, int value) = 0;

	/**
	 * Returns a list of audio samples for current time
	 * \return Vector of paths to audio files representing current time (hours and minutes)
	 */
	virtual std::tuple<std::vector<std::string>, boost::posix_time::ptime> getAudioForCurrentTime(void) = 0;

	/**
	 * Returns a list of audio samples for hour and minute represented by epoch timestamp
	 * @param timetstamp
	 * @param addUniversalTime adds additional sentence "universal time"
	 * @return
	 */
	virtual std::vector<std::string> getAudioForHourAndMinute(uint64_t timetstamp, bool addUniversalTime) = 0;

	/**
	 * Returns a file for given constant announcement element, or empty optional if
	 * it cannot be found.
	 * \return Optional path to file representing constant element of an announcement, or
	 * 		   empty if no file could be found
	 */
	virtual std::optional<std::string> getConstantElement(PlaylistSampler_ConstanElement element) = 0;

	/**
	 * Returns a list of audio samples for forecast announcement like
	 * "Forecast for next 3 hours". If audio cannot be generated (wrong time step)
	 * empty optional is returned
	 */
	virtual std::optional<std::vector<std::string>> getAudioForForecastAnouncement(int minutes) = 0;

	/**
	 *
	 * @param hours
	 * @param change
	 * @param unit
	 * @return
	 */
	virtual std::vector<std::string> getAudioForTrendAnouncement(int hours, float change, PlaylistSampler_Unit unit) = 0;

	/**
	 * Returns an audio file with given station name. If the station is unknown
	 * enpty optional is returned
	 *
	 * \param name weather station or forecast point name
	 */
	virtual std::optional<std::string> getAudioForStationName(std::string name) = 0;

	/**
	 * Returns an audio file with given forecast name. If the station is unknown
	 * enpty optional is returned
	 *
	 * \param name weather station or forecast point name
	 */
	virtual std::optional<std::string> getAudioForForecastName(std::string name) = 0;

	/**
	 *
	 */
	virtual std::optional<std::string> getAudioForAvalancheWarningLocation(AvalancheWarnings_Location location) = 0;

	/**
	 * Converts wind direction to wind direction name
	 *
	 * \param direction
	 */
	virtual std::string getAudioForWindDirection(int direction) = 0;

	/**
	 * Converts a single word into NATO phonetic pronunciation. This method
	 * should be universal across all implementations as the NATO phonetic
	 * alphabet is the same, although each language will have distinct
	 * voice used to generate samples. "BRAVO" will be the same word in each
	 * case, although it may sound a little bit differently across different
	 * natural languages, which shall be taken into consideration not to
	 * confuse a listener.
	 * @param word
	 * @return vector of filenames with each letter spoken phonetically
	 */
	virtual std::vector<std::string> getPhoneticForWord(std::string word) = 0;

#ifdef PANSA_AIRSPACE_ENABLED
	virtual std::string getForAirspaceType(PansaAirspace_Type type) = 0;

	virtual std::string getAirspaceConstantElement(const PlaylistSampler_Airspace _airspace) = 0;
#endif

	/**
	 *
	 */
//	PlaylistCreator();
	virtual ~PlaylistSampler() {

	}
//	PlaylistCreator(const PlaylistCreator &other);
//	PlaylistCreator(PlaylistCreator &&other);
//	PlaylistCreator& operator=(const PlaylistCreator &other);
//	PlaylistCreator& operator=(PlaylistCreator &&other);
};

#endif /* PLAYLISTSAMPLER_H_ */

/*
 * PlaylistCreator.h
 *
 *  Created on: Dec 30, 2022
 *      Author: mateusz
 */

#ifndef PLAYLISTCREATOR_H_
#define PLAYLISTCREATOR_H_

#include <vector>
#include <string>
#include <optional>
#include <tuple>

#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o

/**
 * Enumeration representing different measurement units.
 */
enum PlaylistCreator_Unit {
	KNOTS,
	MS,		//!< Meters per second
	DEG,	//!< Degrees (like for wind direction)
	PERCENT,
	CELSIUS,//!< Degrees Celsius
	HPA		//!<
};

/**
 * Constant element of an anouncement.
 */
enum PlaylistCreator_ConstanElement {
	CURRENT_TIME,	//!< like "Current time is"
	CURRENT_WEATHER,//!< "Current weather conditions"
	FORECAST,		//!< "Forecast for next"
	HOURS			//!< "hours"
};

class PlaylistCreator {

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
	 * \return Path to audio file
	 */
	virtual std::string getAudioFromUnit(PlaylistCreator_Unit unit) = 0;

	/**
	 * Returns a list of audio samples for current time
	 * \return Vector of paths to audio files representing current time (hours and minutes)
	 */
	virtual std::tuple<std::vector<std::string>, boost::posix_time::ptime> getAudioForCurrentTime(void) = 0;

	/**
	 * Returns a file for given constant announcement element, or empty optional if
	 * it cannot be found.
	 * \return Optional path to file representing constant element of an announcement, or
	 * 		   empty if no file could be found
	 */
	virtual std::optional<std::string> getConstantElement(PlaylistCreator_ConstanElement element) = 0;

	/**
	 * Returns a list of audio samples for forecast announcement like
	 * "Forecast for next 3 hours". If audio cannot be generated (wrong time step)
	 * empty optional is returned
	 */
	virtual std::optional<std::vector<std::string>> getAudioForForecastAnouncement(int minutes) = 0;

	/**
	 * Returns an audio file with given station name or forecast name. If the station is unknown
	 * enpty optional is returned
	 *
	 * \param name weather station or forecast point name
	 */
	virtual std::optional<std::string> getAudioForStationName(std::string name) = 0;

	/**
	 *
	 */
//	PlaylistCreator();
	virtual ~PlaylistCreator() {

	}
//	PlaylistCreator(const PlaylistCreator &other);
//	PlaylistCreator(PlaylistCreator &&other);
//	PlaylistCreator& operator=(const PlaylistCreator &other);
//	PlaylistCreator& operator=(PlaylistCreator &&other);
};

#endif /* PLAYLISTCREATOR_H_ */

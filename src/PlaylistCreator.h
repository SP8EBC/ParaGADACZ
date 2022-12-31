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
public:

	/**
	 * Returns a list of audio files representing given integer number (may be negative).
	 * As an example, number -123 will return in English:
	 * 		minus.mp3
	 * 		one.mp3
	 *		hundret.mp3
	 *		twenty.mp3
	 *		three.mp3
	 */
	std::vector<std::string>& getAudioListFromNumber(int integer);

	/**
	 * Returns a list of audio files representing given floating point number.
	 * As an example, number -1.45 will return in English:
	 * 		minus.mp3
	 * 		one.mp3
	 * 		decimal.mp3
	 * 		fourty.mp3
	 * 		five.mp3
	 */
	std::vector<std::string>& getAudioListFromNumber(float decimal);

	/**
	 * Returns single path to audio file
	 */
	std::string& getAudioFromUnit(PlaylistCreator_Unit unit);

	/**
	 * Returns a list of audio samples for current time
	 */
	std::vector<std::string>& getAudioForCurrentTime(void);

	std::string& getConstantElement();

	/**
	 * \param locale chose a locale which should be used to create audio playlist
	 */
	PlaylistCreator(std::string locale);
	virtual ~PlaylistCreator();
	PlaylistCreator(const PlaylistCreator &other);
	PlaylistCreator(PlaylistCreator &&other);
	PlaylistCreator& operator=(const PlaylistCreator &other);
	PlaylistCreator& operator=(PlaylistCreator &&other);
};

#endif /* PLAYLISTCREATOR_H_ */

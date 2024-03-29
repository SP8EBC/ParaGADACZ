/*
 * PlaylistCreatorPL.h
 *
 *  Created on: Dec 31, 2022
 *      Author: mateusz
 */

#ifndef PLAYLISTSAMPLERPL_H_
#define PLAYLISTSAMPLERPL_H_

#include "ConfigurationFile.h"
#include "PlaylistSampler.h"

/**
 * This class is a specific implementation of PlaylistCreator for Polish
 * language.
 */
class PlaylistSamplerPL: public virtual PlaylistSampler {

	const std::shared_ptr<ConfigurationFile> & config;

public:
	PlaylistSamplerPL(std::shared_ptr<ConfigurationFile> & configurationFile);
	virtual ~PlaylistSamplerPL();
	PlaylistSamplerPL(const PlaylistSamplerPL &other);
	PlaylistSamplerPL(PlaylistSamplerPL &&other);

	// this is crap is generated by Eclipse 'implement method' function.
	// it looks ugly but I have no time to fight with that.

	/**
	 * Returns a list of audio samples for current time
	 * \return Vector of paths to audio files representing current time (hours and minutes)
	 */
	virtual std::tuple<
			std::vector<
					std::__cxx11::basic_string<char, std::char_traits<char>,
							std::allocator<char> >,
					std::allocator<
							std::__cxx11::basic_string<char,
									std::char_traits<char>, std::allocator<char> > > >,
			boost::posix_time::ptime> getAudioForCurrentTime() ;

	/**
	 * Returns a list of audio samples for hour and minute represented by epoch timestamp
	 * @param timetstamp
	 * @param addUniversalTime adds additional sentece "universal time"
	 * @return
	 */
	virtual std::vector<
			std::__cxx11::basic_string<char, std::char_traits<char>,
					std::allocator<char> >,
			std::allocator<
					std::__cxx11::basic_string<char, std::char_traits<char>,
							std::allocator<char> > > > getAudioForHourAndMinute(
			uint64_t timetstamp, bool addUniversalTime);

	/**
	 * Returns a list of audio samples for forecast announcement like
	 * "Forecast for next 3 hours". If audio cannot be generated (wrong time step)
	 * empty optional is returned
	 */
	virtual std::optional<
			std::vector<
					std::__cxx11::basic_string<char, std::char_traits<char>,
							std::allocator<char> >,
					std::allocator<
							std::__cxx11::basic_string<char,
									std::char_traits<char>, std::allocator<char> > > > > getAudioForForecastAnouncement(
			int minutes);

	/**
	 *
	 * @param hours
	 * @param change
	 * @param unit
	 * @return
	 */
	virtual std::vector<std::string> getAudioForTrendAnouncement(int hours, float change, PlaylistSampler_Unit unit);


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
	virtual std::vector<
			std::__cxx11::basic_string<char, std::char_traits<char>,
					std::allocator<char> >,
			std::allocator<
					std::__cxx11::basic_string<char, std::char_traits<char>,
							std::allocator<char> > > > getAudioListFromNumber(
			float decimal);

	/**
	 * Returns an audio file with given station name or forecast name. If the station is unknown
	 * enpty optional is returned
	 *
	 * \param name weather station or forecast point name
	 */
	virtual std::optional<
			std::__cxx11::basic_string<char, std::char_traits<char>,
					std::allocator<char> > > getAudioForStationName(
			std::string name);

	/**
	 * Returns an audio file with given forecast name. If the station is unknown
	 * enpty optional is returned
	 *
	 * \param name weather station or forecast point name
	 */
	virtual std::optional<std::string> getAudioForForecastName(std::string name);

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
	virtual std::vector<
			std::__cxx11::basic_string<char, std::char_traits<char>,
					std::allocator<char> >,
			std::allocator<
					std::__cxx11::basic_string<char, std::char_traits<char>,
							std::allocator<char> > > > getAudioListFromNumber(
			int integer);

	/**
	 * Returns single path to audio file
	 *
	 * \param unit Measurement unit to get audio file for
	 * \param value Needed for language which has declination depending on number
	 * \return Path to audio file
	 */
	virtual std::string getAudioFromUnit(PlaylistSampler_Unit unit, int value);

	/**
	 * Returns a file for given constant announcement element, or empty optional if
	 * it cannot be found.
	 * \return Optional path to file representing constant element of an announcement, or
	 * 		   empty if no file could be found
	 */
	virtual std::optional<
			std::__cxx11::basic_string<char, std::char_traits<char>,
					std::allocator<char> > > getConstantElement(
			PlaylistSampler_ConstanElement element);

	/**
	 * Converts wind direction to wind direction name
	 *
	 * \param direction
	 */
	virtual std::string getAudioForWindDirection(int direction);
	virtual std::optional<
			std::__cxx11::basic_string<char, std::char_traits<char>,
					std::allocator<char> > > getAudioForAvalancheWarningLocation(
			AvalancheWarnings_Location location) override;

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
	virtual std::vector<
			std::__cxx11::basic_string<char, std::char_traits<char>,
					std::allocator<char> >,
			std::allocator<
					std::__cxx11::basic_string<char, std::char_traits<char>,
							std::allocator<char> > > > getPhoneticForWord(
			std::string word);

#ifdef PANSA_AIRSPACE_ENABLED
	/**
	 *
	 * @param type
	 * @return
	 */
	virtual std::string getForAirspaceType(PansaAirspace_Type type) override;

	virtual std::string getAirspaceConstantElement(
			PlaylistSampler_Airspace _airspace);
#endif
};

#endif /* PLAYLISTSAMPLERPL_H_ */

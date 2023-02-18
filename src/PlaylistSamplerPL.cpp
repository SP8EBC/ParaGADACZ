/*
 * PlaylistCreatorPL.cpp
 *
 *  Created on: Dec 31, 2022
 *      Author: mateusz
 */

#include "PlaylistSamplerPL.h"
#include "PlaylistSamplerPL_files.h"

#include <boost/algorithm/string.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <stdexcept>

PlaylistSamplerPL::PlaylistSamplerPL(std::shared_ptr<ConfigurationFile> & configurationFile) : config(configurationFile) {

}

PlaylistSamplerPL::~PlaylistSamplerPL() {
	// TODO Auto-generated destructor stub
}

PlaylistSamplerPL::PlaylistSamplerPL(const PlaylistSamplerPL &other) : config(other.config) {
	// TODO Auto-generated constructor stub

}

PlaylistSamplerPL::PlaylistSamplerPL(PlaylistSamplerPL &&other) : config(other.config) {
	// TODO Auto-generated constructor stub

}

// this method implementations have been generated automatically by Eclipse
// and it's 'implement method' function. what I should say? Eclipse being
// Eclipse one more time ?

/**
 * Returns a list of audio samples for current time
 * \return Vector of paths to audio files representing current time (hours and minutes)
 */
std::tuple<
		std::vector<
				std::__cxx11::basic_string<char, std::char_traits<char>,
						std::allocator<char> >,
				std::allocator<
						std::__cxx11::basic_string<char,
								std::char_traits<char>, std::allocator<char> > > >,
		boost::posix_time::ptime> PlaylistSamplerPL::getAudioForCurrentTime() {

	// get current time
	boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

	// get time
	const boost::posix_time::time_duration time_of_day = current.time_of_day();

	// get hours
	const uint8_t hours = time_of_day.hours();

	// get minutes
	const int minutes = time_of_day.minutes();

	SPDLOG_INFO("hours: {}, minutes: {}", hours, minutes);

	// playlist for audio samples
	std::vector<std::string> playlist;

	playlist.push_back(TIME);

	// get hours component
	switch (hours) {
	case 0: playlist.push_back(NUMBER_0); SPDLOG_DEBUG("playlist append: {}", NUMBER_0); break;
	case 1: playlist.push_back(ONE_CLOCK); SPDLOG_DEBUG("playlist append: {}", ONE_CLOCK); break;
	case 2: playlist.push_back(TWO_CLOCK); SPDLOG_DEBUG("playlist append: {}", TWO_CLOCK); break;
	case 3: playlist.push_back(THREE_CLOCK); SPDLOG_DEBUG("playlist append: {}", THREE_CLOCK); break;
	case 4: playlist.push_back(FOUR_CLOCK); SPDLOG_DEBUG("playlist append: {}", FOUR_CLOCK); break;
	case 5: playlist.push_back(FIVE_CLOCK); SPDLOG_DEBUG("playlist append: {}", FIVE_CLOCK); break;
	case 6: playlist.push_back(SIX_CLOCK); SPDLOG_DEBUG("playlist append: {}", SIX_CLOCK); break;
	case 7: playlist.push_back(SEVEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", SEVEN_CLOCK); break;
	case 8: playlist.push_back(EIGHT_CLOCK); SPDLOG_DEBUG("playlist append: {}", EIGHT_CLOCK); break;
	case 9: playlist.push_back(NINE_CLOCK); SPDLOG_DEBUG("playlist append: {}", NINE_CLOCK); break;
	case 10: playlist.push_back(TEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", TEN_CLOCK); break;
	case 11: playlist.push_back(ELEVEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", ELEVEN_CLOCK); break;
	case 12: playlist.push_back(TWELVE_CLOCK); SPDLOG_DEBUG("playlist append: {}", TWELVE_CLOCK); break;
	case 13: playlist.push_back(THIRTEEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", THIRTEEN_CLOCK); break;
	case 14: playlist.push_back(FOURTEEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", FOURTEEN_CLOCK); break;
	case 15: playlist.push_back(FIFVETEEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", FIFVETEEN_CLOCK); break;
	case 16: playlist.push_back(SIXTEEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", SIXTEEN_CLOCK); break;
	case 17: playlist.push_back(SEVENTEEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", SEVENTEEN_CLOCK); break;
	case 18: playlist.push_back(EIGHTEEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", EIGHTEEN_CLOCK); break;
	case 19: playlist.push_back(NINETEEN_CLOCK); SPDLOG_DEBUG("playlist append: {}", NINETEEN_CLOCK); break;
	case 20: playlist.push_back(TWENTY_CLOCK); SPDLOG_DEBUG("playlist append: {}", TWENTY_CLOCK); break;
	case 21: playlist.push_back(TWENTY_CLOCK); playlist.push_back(ONE_CLOCK); break;
	case 22: playlist.push_back(TWENTY_CLOCK); playlist.push_back(TWO_CLOCK); break;
	case 23: playlist.push_back(TWENTY_CLOCK); playlist.push_back(THREE_CLOCK); break;
	}

	// get minutes into separate vector
	std::vector<std::string> minutes_playlist = this->getAudioListFromNumber(minutes);

	if (minutes_playlist.size() > 0) {
		// now merge two vector together
		playlist.insert(playlist.end(), minutes_playlist.begin(), minutes_playlist.end());

		// append the end
		playlist.push_back(UTC);
	}

	std::tuple<std::vector<std::string>, boost::posix_time::ptime> out = std::make_tuple(playlist, current);

	return out;

}

/**
 * Returns a list of audio samples for forecast announcement like
 * "Forecast for next 3 hours". If audio cannot be generated (wrong time step)
 * empty optional is returned
 */
std::optional<
		std::vector<
				std::__cxx11::basic_string<char, std::char_traits<char>,
						std::allocator<char> >,
				std::allocator<
						std::__cxx11::basic_string<char, std::char_traits<char>,
								std::allocator<char> > > > > PlaylistSamplerPL::getAudioForForecastAnouncement(
		int minutes) {


	if ((minutes % 60) != 0) {

		SPDLOG_ERROR("Time step for forecast should be in 1 hour resoulution. minutes: {}", minutes);

		return std::nullopt;
	}
	else {
		std::vector<std::string> out;

		// "prognoza pogody na nastepne"
		out.push_back(FCAST);

		out.push_back(FOR_NEXT);

		switch ((int)(minutes / 60)) {
		case 1: out.push_back(NUMBER_1); out.push_back(HOUR_ONE); break;
		case 2: out.push_back(NUMBER_2_); out.push_back(HOUR_TWO_FOUR); break;
		case 3: out.push_back(NUMBER_3); out.push_back(HOUR_TWO_FOUR);  break;
		case 4: out.push_back(NUMBER_4); out.push_back(HOUR_TWO_FOUR);  break;
		case 5: out.push_back(NUMBER_5); out.push_back(HOUR_FOUR);  break;
		case 6: out.push_back(NUMBER_6); out.push_back(HOUR_FOUR);  break;
		case 7: out.push_back(NUMBER_7); out.push_back(HOUR_FOUR);  break;
		case 8: out.push_back(NUMBER_8); out.push_back(HOUR_FOUR);  break;
		case 9: out.push_back(NUMBER_9); out.push_back(HOUR_FOUR);  break;
		}

		return out;

	}

}

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
std::vector<
		std::__cxx11::basic_string<char, std::char_traits<char>,
				std::allocator<char> >,
		std::allocator<
				std::__cxx11::basic_string<char, std::char_traits<char>,
						std::allocator<char> > > > PlaylistSamplerPL::getAudioListFromNumber(
		float decimal) {

	std::vector<std::string> out;

	SPDLOG_INFO("decimal: {}", decimal);

	float helper = 0.0f;

	// thousands from number
	const int thousands = (int)(decimal / 1000.0f);

	// remaining after extracting thousands
	helper = decimal - 1000.0f * thousands;

	SPDLOG_DEBUG("thousands: {}, helper: {}", thousands, helper);

	// hundrets
	const int hundreds = (int)(helper / 100.0f);

	// remaining after extracting hundrets
	helper = helper - 100.0f * hundreds;

	SPDLOG_DEBUG("hundreds: {}, helper: {}", hundreds, helper);

	// tens
	const int tens = (int)(helper / 10.0f);

	// remaining after tens
	helper = helper - 10.0f * tens;

	SPDLOG_DEBUG("tens: {}, helper: {}", tens, helper);

	// units - we are done with integer part
	const int units = (int) helper;

	// now fractional part (after decimal point)

	// remove integer part
	helper = helper - (float)units;

	SPDLOG_DEBUG("units: {}, helper: {}", units, helper);

	const int one_tens = (int)(helper * 10);

	SPDLOG_DEBUG("one_tens: {}, helper: {}", one_tens, helper);

	// now select files
	switch (thousands) {
	case 1: out.push_back(NUMBER_1k); break;
	case 2: out.push_back(NUMBER_2); out.push_back(NUMBER_2k_4k); break;
	case 3: out.push_back(NUMBER_3); out.push_back(NUMBER_2k_4k); break;
	case 4: out.push_back(NUMBER_4); out.push_back(NUMBER_2k_4k); break;
	case 5: out.push_back(NUMBER_5); out.push_back(NUMBER_5k); break;
	case 6: out.push_back(NUMBER_6); out.push_back(NUMBER_5k); break;
	case 7: out.push_back(NUMBER_7); out.push_back(NUMBER_5k); break;
	case 8: out.push_back(NUMBER_8); out.push_back(NUMBER_5k); break;
	case 9: out.push_back(NUMBER_9); out.push_back(NUMBER_5k); break;
	default: break;
	}

	switch (hundreds) {
	case 1: out.push_back(NUMBER_100); break;
	case 2: out.push_back(NUMBER_200); break;
	case 3: out.push_back(NUMBER_300); break;
	case 4: out.push_back(NUMBER_400); break;
	case 5: out.push_back(NUMBER_500); break;
	case 6: out.push_back(NUMBER_600); break;
	case 7: out.push_back(NUMBER_700); break;
	case 8: out.push_back(NUMBER_800); break;
	case 9: out.push_back(NUMBER_900); break;
	default: break;

	}

	switch (tens) {
		case 2: out.push_back(NUMBER_20); break;
		case 3: out.push_back(NUMBER_30); break;
		case 4: out.push_back(NUMBER_40); break;
		case 5: out.push_back(NUMBER_50); break;
		case 6: out.push_back(NUMBER_60); break;
		case 7: out.push_back(NUMBER_70); break;
		case 8: out.push_back(NUMBER_80); break;
		case 9: out.push_back(NUMBER_90); break;

		// special case for number in range 10-19
		case 1: {
			switch (units) {
			case 1: out.push_back(NUMBER_11); break;
			case 2: out.push_back(NUMBER_12); break;
			case 3: out.push_back(NUMBER_13); break;
			case 4: out.push_back(NUMBER_14); break;
			case 5: out.push_back(NUMBER_15); break;
			case 6: out.push_back(NUMBER_16); break;
			case 7: out.push_back(NUMBER_17); break;
			case 8: out.push_back(NUMBER_18); break;
			case 9: out.push_back(NUMBER_19); break;
			case 0: out.push_back(NUMBER_10); break;

			}
		}
	}

	// add tens digit if this isn't a number from 10...19 range
	if (tens > 1) {
		switch (units) {
		//case 0: out.push_back(NUMBER_0); break;
		case 1: out.push_back(NUMBER_1); break;
		case 2: out.push_back(NUMBER_2); break;
		case 3: out.push_back(NUMBER_3); break;
		case 4: out.push_back(NUMBER_4); break;
		case 5: out.push_back(NUMBER_5); break;
		case 6: out.push_back(NUMBER_6); break;
		case 7: out.push_back(NUMBER_7); break;
		case 8: out.push_back(NUMBER_8); break;
		case 9: out.push_back(NUMBER_9); break;
		}
	}
	else if (tens == 0){
		switch (units) {
		case 0: out.push_back(NUMBER_0); break;
		case 1: out.push_back(NUMBER_1); break;
		case 2: out.push_back(NUMBER_2); break;
		case 3: out.push_back(NUMBER_3); break;
		case 4: out.push_back(NUMBER_4); break;
		case 5: out.push_back(NUMBER_5); break;
		case 6: out.push_back(NUMBER_6); break;
		case 7: out.push_back(NUMBER_7); break;
		case 8: out.push_back(NUMBER_8); break;
		case 9: out.push_back(NUMBER_9); break;
		}
	}
	else {
		;	// already added for range 10...19 in tens switch case 1
	}

	// if there is any decimal part
	if (one_tens > 0) {
		out.push_back(DECIMAL);

		switch (one_tens) {
		case 1: out.push_back(NUMBER_1); break;
		case 2: out.push_back(NUMBER_2); break;
		case 3: out.push_back(NUMBER_3); break;
		case 4: out.push_back(NUMBER_4); break;
		case 5: out.push_back(NUMBER_5); break;
		case 6: out.push_back(NUMBER_6); break;
		case 7: out.push_back(NUMBER_7); break;
		case 8: out.push_back(NUMBER_8); break;
		case 9: out.push_back(NUMBER_9); break;
		}
	}

	return out;
}

/**
 * Returns an audio file with given station name or forecast name. If the station is unknown
 * enpty optional is returned
 *
 * \param name weather station or forecast point name
 */
std::optional<
		std::__cxx11::basic_string<char, std::char_traits<char>,
				std::allocator<char> > > PlaylistSamplerPL::getAudioForStationName(
		std::string name) {

	std::optional<std::string> out = std::nullopt;

	boost::to_upper(name);

	// get meteoblue forecast configuration
	const std::vector<ConfigurationFile_CurrentWeather> & stations = config->getCurrent();

	// iterate through all configuration
	for (auto station : stations) {

		// get a copy of a name of this station
		std::string location_name = station.name;

		// convert uppercase
		boost::to_upper(location_name);

		// check if this is the thing we're looking for
		if (location_name == name) {
			out = station.fnIdent;

			SPDLOG_INFO("Found audio anonucement {} for station {}", station.name, station.fnIdent);
		}
	}

	return out;
}

/**
 * Returns an audio file with given forecast name. If the station is unknown
 * enpty optional is returned
 *
 * \param name weather station or forecast point name
 */
std::optional<std::string> PlaylistSamplerPL::getAudioForForecastName(std::string name)
{
	std::optional<std::string> out = std::nullopt;

	boost::to_upper(name);

	// get meteoblue forecast configuration
	const ConfigurationFile_ForecastMeteoblue & forecasts = config->getForecast();

	// iterate through all configuration
	for (auto location : forecasts.locations) {
		// get a copy of a name of this station
		std::string location_name = location.name;

		// convert uppercase
		boost::to_upper(location_name);

		// check if this is the thing we're looking for
		if (location_name == name) {
			out = location.nameIdent;

			SPDLOG_INFO("Found audio anonucement {} for meteoblue fotecast {}", name, location.nameIdent);
		}
	}

	return out;
}

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
std::vector<
		std::__cxx11::basic_string<char, std::char_traits<char>,
				std::allocator<char> >,
		std::allocator<
				std::__cxx11::basic_string<char, std::char_traits<char>,
						std::allocator<char> > > > PlaylistSamplerPL::getAudioListFromNumber(
		int integer) {

	std::vector<std::string> out;

	SPDLOG_INFO("integer: {}", integer);

	float helper = 0.0f;

	// thousands from number
	const int thousands = (int)(integer / 1000.0f);

	// remaining after extracting thousands
	helper = integer - 1000.0f * thousands;

	SPDLOG_DEBUG("thousands: {}, helper: {}", thousands, helper);

	// hundrets
	const int hundreds = (int)(helper / 100.0f);

	// remaining after extracting hundrets
	helper = helper - 100.0f * hundreds;

	SPDLOG_DEBUG("hundreds: {}, helper: {}", hundreds, helper);

	// tens
	const int tens = (int)(helper / 10.0f);

	// remaining after tens
	helper = helper - 10.0f * tens;

	SPDLOG_DEBUG("tens: {}, helper: {}", tens, helper);

	// units - we are done with integer part
	const int units = (int) helper;

	SPDLOG_DEBUG("units: {}", units);

	// now select files
	switch (thousands) {
	case 1: out.push_back(NUMBER_1k); break;
	case 2: out.push_back(NUMBER_2); out.push_back(NUMBER_2k_4k); break;
	case 3: out.push_back(NUMBER_3); out.push_back(NUMBER_2k_4k); break;
	case 4: out.push_back(NUMBER_4); out.push_back(NUMBER_2k_4k); break;
	case 5: out.push_back(NUMBER_5); out.push_back(NUMBER_5k); break;
	case 6: out.push_back(NUMBER_6); out.push_back(NUMBER_5k); break;
	case 7: out.push_back(NUMBER_7); out.push_back(NUMBER_5k); break;
	case 8: out.push_back(NUMBER_8); out.push_back(NUMBER_5k); break;
	case 9: out.push_back(NUMBER_9); out.push_back(NUMBER_5k); break;
	default: break;
	}

	switch (hundreds) {
	case 1: out.push_back(NUMBER_100); break;
	case 2: out.push_back(NUMBER_200); break;
	case 3: out.push_back(NUMBER_300); break;
	case 4: out.push_back(NUMBER_400); break;
	case 5: out.push_back(NUMBER_500); break;
	case 6: out.push_back(NUMBER_600); break;
	case 7: out.push_back(NUMBER_700); break;
	case 8: out.push_back(NUMBER_800); break;
	case 9: out.push_back(NUMBER_900); break;
	default: break;

	}

	switch (tens) {
		case 2: out.push_back(NUMBER_20); break;
		case 3: out.push_back(NUMBER_30); break;
		case 4: out.push_back(NUMBER_40); break;
		case 5: out.push_back(NUMBER_50); break;
		case 6: out.push_back(NUMBER_60); break;
		case 7: out.push_back(NUMBER_70); break;
		case 8: out.push_back(NUMBER_80); break;
		case 9: out.push_back(NUMBER_90); break;

		// special case for number in range 10-19
		case 1: {
			switch (units) {
			case 1: out.push_back(NUMBER_11); break;
			case 2: out.push_back(NUMBER_12); break;
			case 3: out.push_back(NUMBER_13); break;
			case 4: out.push_back(NUMBER_14); break;
			case 5: out.push_back(NUMBER_15); break;
			case 6: out.push_back(NUMBER_16); break;
			case 7: out.push_back(NUMBER_17); break;
			case 8: out.push_back(NUMBER_18); break;
			case 9: out.push_back(NUMBER_19); break;
			case 0: out.push_back(NUMBER_10); break;

			}
		}
	}

	// add tens digit if this isn't a number from 10...19 range
	if (tens > 1) {
		switch (units) {
		//case 0: out.push_back(NUMBER_0); break;
		case 1: out.push_back(NUMBER_1); break;
		case 2: out.push_back(NUMBER_2); break;
		case 3: out.push_back(NUMBER_3); break;
		case 4: out.push_back(NUMBER_4); break;
		case 5: out.push_back(NUMBER_5); break;
		case 6: out.push_back(NUMBER_6); break;
		case 7: out.push_back(NUMBER_7); break;
		case 8: out.push_back(NUMBER_8); break;
		case 9: out.push_back(NUMBER_9); break;
		}
	}
	else if (tens == 0){
		switch (units) {
		case 0: out.push_back(NUMBER_0); break;
		case 1: out.push_back(NUMBER_1); break;
		case 2: out.push_back(NUMBER_2); break;
		case 3: out.push_back(NUMBER_3); break;
		case 4: out.push_back(NUMBER_4); break;
		case 5: out.push_back(NUMBER_5); break;
		case 6: out.push_back(NUMBER_6); break;
		case 7: out.push_back(NUMBER_7); break;
		case 8: out.push_back(NUMBER_8); break;
		case 9: out.push_back(NUMBER_9); break;
		}
	}
	else {
		;	// already added for range 10...19 in tens switch case 1
	}

	return out;
}

/**
 * Returns single path to audio file
 *
 * \param unit Measurement unit to get audio file for
 * \param value Needed for language which has declination depending on number
 * \return Path to audio file
 */
std::string PlaylistSamplerPL::getAudioFromUnit(PlaylistSampler_Unit unit, int value) {

	std::string out;

	if (unit == PlaylistSampler_Unit::PERCENT) {
		// percents doesn't depends on value, it always "procent"
		out = PERCENTS;
	}
	else if (unit == PlaylistSampler_Unit::HPA) {
		// the same with hectopascals. it will be always >>>> 4
		out = HECTOPASCALS;
	}
	else if (unit == PlaylistSampler_Unit::CELSIUS) {
		// celsius will be second word after
		out = CELSIUSS;
	}
	else {
		if ( value == 1) {
			switch (unit) {
			case PlaylistSampler_Unit::KNOTS:	out = KNOT_ONE; 		break;
			case PlaylistSampler_Unit::MS:		out = MS_ONE;			break;
			case PlaylistSampler_Unit::DEG:		out = DEGREE_ONE;		break;
			default: break;
			}
		}
		else if ( value == 2 || value == 3 || value == 4) {
			switch (unit) {
			case PlaylistSampler_Unit::KNOTS:	out = KNOT_TWO_FOUR; 		break;
			case PlaylistSampler_Unit::MS:		out = MS_TWO_FOUR;			break;
			case PlaylistSampler_Unit::DEG:		out = DEGREE_TWO_FOUR;		break;
			default: break;
			}
		}
		else {
			switch (unit) {
			case PlaylistSampler_Unit::KNOTS:	out = KNOT_FOUR; 		break;
			case PlaylistSampler_Unit::MS:		out = MS_FOUR;			break;
			case PlaylistSampler_Unit::DEG:		out = DEGREE_FOUR;		break;
			default: break;
			}
		}
	}

	return out;
}

/**
 * Returns a file for given constant announcement element, or empty optional if
 * it cannot be found.
 * \return Optional path to file representing constant element of an announcement, or
 * 		   empty if no file could be found
 */
std::optional<
		std::__cxx11::basic_string<char, std::char_traits<char>,
				std::allocator<char> > > PlaylistSamplerPL::getConstantElement(
		PlaylistSampler_ConstanElement element) {

	switch (element) {
	case PlaylistSampler_ConstanElement::CURRENT_TIME:	return TIME;			//!< "godzina"
	case PlaylistSampler_ConstanElement::CURRENT_WEATHER:	return WEATHER;		//!< "Aktualne Warunki"
	case PlaylistSampler_ConstanElement::REGIONAL_QNH: 		return QNH_REGIONAL;//!< "Ciśnienie atmosferyczne w regionie"
	case PlaylistSampler_ConstanElement::FORECAST:		return FCAST;			//!< "Prognoza na nastepne"
	case PlaylistSampler_ConstanElement::HOURS:			return HOUR_TWO_FOUR;	//!< "hours"
	case PlaylistSampler_ConstanElement::WIND: 			return DIRECTION;		//!< "wiatr"
	case PlaylistSampler_ConstanElement::TEMPERATURE: 	return TMPRATURE;
	case PlaylistSampler_ConstanElement::HUMIDITY:		return HMIDITY;
	case PlaylistSampler_ConstanElement::WIND_GUSTS:		return WINDGUSTS;
	}

	SPDLOG_ERROR("Unknown element: {}", element);

	return {};
}

/**
 * Converts wind direction to wind direction name
 *
 * \param direction
 */
std::string PlaylistSamplerPL::getAudioForWindDirection(int direction) {

	if ((direction <= 11 && direction >= 0) || (direction > 349 && direction < 360))
		return DIRECTION_N;
	else if (direction <= 34 && direction > 11)
		return DIRECTION_NNE;
	else if (direction <= 56 && direction > 34)
		return DIRECTION_NE;
	else if (direction <= 79 && direction > 56)
		return DIRECTION_ENE;
	else if (direction <= 101 && direction > 79)
		return DIRECTION_E;
	else if (direction <= 124 && direction > 101)
		return DIRECTION_ESE;
	else if (direction <= 146 && direction > 124)
		return DIRECTION_SE;
	else if (direction <= 169 && direction > 146)
		return DIRECTION_SSE;
	else if (direction <= 191 && direction > 169)
		return DIRECTION_S;
	else if (direction <= 214 && direction > 191)
		return DIRECTION_SSW;
	else if (direction <= 236 && direction > 214)
		return DIRECTION_SW;
	else if (direction <= 259 && direction > 236)
		return DIRECTION_WSW;
	else if (direction <= 281 && direction > 259)
		return DIRECTION_W;
	else if (direction <= 304 && direction > 281)
		return DIRECTION_WNW;
	else if (direction <= 327 && direction > 304)
		return DIRECTION_NW;
	else if (direction <= 349 && direction > 327)
		return DIRECTION_NNW;
	else {
		SPDLOG_ERROR("Unknown wind direction, directon {}", direction);
		throw std::range_error("");
		//return "";
	}
}

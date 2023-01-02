/*
 * PlaylistCreatorPL.cpp
 *
 *  Created on: Dec 31, 2022
 *      Author: mateusz
 */

#include "PlaylistCreatorPL.h"
#include "PlaylistCreatorPL_files.h"

#undef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

PlaylistCreatorPL::PlaylistCreatorPL() {
	// TODO Auto-generated constructor stub

}

PlaylistCreatorPL::~PlaylistCreatorPL() {
	// TODO Auto-generated destructor stub
}

PlaylistCreatorPL::PlaylistCreatorPL(const PlaylistCreatorPL &other) {
	// TODO Auto-generated constructor stub

}

PlaylistCreatorPL::PlaylistCreatorPL(PlaylistCreatorPL &&other) {
	// TODO Auto-generated constructor stub

}

PlaylistCreatorPL& PlaylistCreatorPL::operator=(
		const PlaylistCreatorPL &other) {
	// TODO Auto-generated method stub

}

PlaylistCreatorPL& PlaylistCreatorPL::operator=(PlaylistCreatorPL &&other) {
	// TODO Auto-generated method stub

}

// this method implementations have been generated automatically by Eclipse
// and it's 'implement method' function. what I should say? Eclipse being
// Eclipse one more time ?

/**
 * Returns a list of audio samples for current time
 * \return Vector of paths to audio files representing current time (hours and minutes)
 */
std::vector<
		std::__cxx11::basic_string<char, std::char_traits<char>,
				std::allocator<char> >,
		std::allocator<
				std::__cxx11::basic_string<char, std::char_traits<char>,
						std::allocator<char> > > > PlaylistCreatorPL::getAudioForCurrentTime() {
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
								std::allocator<char> > > > > PlaylistCreatorPL::getAudioForForecastAnouncement(
		int minutes) {
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
						std::allocator<char> > > > PlaylistCreatorPL::getAudioListFromNumber(
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

	// add tens if this isn't a number from 10...19 range
	if (tens != 1) {
		switch (units) {
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
				std::allocator<char> > > PlaylistCreatorPL::getAudioForStationName(
		std::string name) {
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
						std::allocator<char> > > > PlaylistCreatorPL::getAudioListFromNumber(
		int integer) {
}

/**
 * Returns single path to audio file
 *
 * \param unit Measurement unit to get audio file for
 * \return Path to audio file
 */
std::string PlaylistCreatorPL::getAudioFromUnit(PlaylistCreator_Unit unit) {
}

/**
 * Returns a file for given constant announcement element, or empty optional if
 * it cannot be found.
 * \return Optional path to file representing constant element of an announcement, or
 * 		   empty if no file could be found
 */
std::optional<
		std::__cxx11::basic_string<char, std::char_traits<char>,
				std::allocator<char> > > PlaylistCreatorPL::getConstantElement(
		PlaylistCreator_ConstanElement element) {
}

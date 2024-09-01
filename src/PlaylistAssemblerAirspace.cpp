/*
 * PlaylistAssemblerAirspace.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: mateusz
 */

#include "PlaylistAssemblerAirspace.h"
#include "TimeTools.h"

#ifdef PANSA_AIRSPACE_ENABLED

#include <regex>
#include <algorithm>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")


PlaylistAssemblerAirspace::PlaylistAssemblerAirspace(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config) :
														hasSomethingToSay(false), sampler(sampler), config(config) {
	// TODO Auto-generated constructor stub

}

/**
 * Checks if a kind of airspace (like TRA, TSA etc) is currently enabled by user configuration
 * @param type airspace type
 * @param config parsed from configuration file
 * @return
 */
bool PlaylistAssemblerAirspace::checkIfAirspaceTypeEnabled(
		PansaAirspace_Type type,
		const ConfigurationFile_Airspace_SayConfigPerElemType &config) {

	bool out = false;

	switch (type) {
		case AIRSPACE_TRA:
			if (config.sayTRA) out = true;
			break;
		case AIRSPACE_TSA:
			if (config.sayTSA) out = true;
			break;
		case AIRSPACE_ATZ:
			if (config.sayATZ) out = true;
			break;
		case AIRSPACE_D:
			if (config.sayD) out = true;
			break;
		case AIRSPACE_R:
			if (config.sayR) out = true;
			break;
		case AIRSPACE_P:
			if (config.sayP) out = true;
			break;
		case AIRSPACE_ADHOC:
			if (config.sayTSA) out = true;
			break;
	}

	return out;
}

/**
 *
 * @param designator
 * @param filter
 * @return
 */
bool PlaylistAssemblerAirspace::checkIfFiltered(const std::string & designator, const std::vector<std::string> & filter) {
	bool out = std::any_of(
			filter.begin(),	// begining of vector with filtering strings
			filter.end(),	// end of vector with filtering strings
			[&designator](const std::string f) {
								// filtering works by looking for a case sensitive
								// substring with a defined filter, within an airspace
								// designator
								const std::size_t found = designator.find(f, 0);
								if (found != std::string::npos) {
									return true;	// returning true ends a loop
								}
								else {
									return false;
								}
							});

	return out;
}

/**
 *
 * @param extractFrom full-designator-string to extract from
 * @param extractWhat true to extract airspace designator or false to extract sector (if applicable)
 * @param type of an airspace. required to look for certain regular expression, which may be different for different
 * @param confPerElemType
 * @return
 */
std::optional<std::string> PlaylistAssemblerAirspace::extractUsingRegexp(
		std::string extractFrom, bool extractWhat, const PansaAirspace_Type type,
		const ConfigurationFile_Airspace_SayConfigPerElemType & confPerElemType) {

	// regular expression used to extract designator or (optional) sector from full-designator-string
	std::regex rexex;

	// results of matching designator or (optional) sector from full-designator-string
    std::smatch match;

    bool result = false;

    std::string usedRegex;

	// extract designator and optional sector information
	switch (type) {
		case AIRSPACE_TRA:
			if (extractWhat) {
				usedRegex = confPerElemType.traDesignatorRegexp;
			}
			else {
				usedRegex = confPerElemType.traSectorRegexp;
			}

			break;
		case AIRSPACE_TSA:
			if (extractWhat) {
				usedRegex = confPerElemType.tsaDesignatorRegexp;
			}
			else {
				usedRegex = confPerElemType.tsaSectorRegexp;
			}

			break;
		case AIRSPACE_ATZ:
			if (extractWhat) {
				usedRegex = confPerElemType.atzDesignatorRegexp;
			}
			else {
				usedRegex = confPerElemType.atzSectorRegexp;
			}

			break;
		case AIRSPACE_D:
			if (extractWhat) {
				usedRegex = confPerElemType.dDesignatorRegexp;
			}
			else {
				usedRegex = confPerElemType.dSectorRegexp;
			}

			break;
		case AIRSPACE_R:
			if (extractWhat) {
				usedRegex = confPerElemType.rDesignatorRegexp;
			}
			else {
				;
			}

			break;
		case AIRSPACE_P:
			if (extractWhat) {
				usedRegex = confPerElemType.pDesignatorRegexp;
			}
			else {
				;
			}

			break;
		case AIRSPACE_ADHOC:
			if (extractWhat) {
				usedRegex = confPerElemType.tsaDesignatorRegexp;
			}
			else {
				usedRegex = confPerElemType.tsaSectorRegexp;
			}

			break;
	}

	// check if any regular expression has been found for input configuration
	if (usedRegex.size() > 0) {

		try {
			// try to create an instance of regex class for this regular expression
			rexex = std::regex(usedRegex, std::regex::ECMAScript);
		}
		catch (std::regex_error & ex) {
			SPDLOG_ERROR("std::regex_error thrown during creating an instance of regex with input regexp {}", usedRegex);
			SPDLOG_ERROR(ex.what());
			result = false;
			return std::nullopt;
		}

		try {
			// parse input string if an instance of regex is created
			result = 	std::regex_search(extractFrom, match, rexex);
		}
		catch (std::regex_error & ex) {
			SPDLOG_ERROR("std::regex_error throw during parsing {} with regexp {}", extractFrom, usedRegex);
			SPDLOG_ERROR(ex.what());
			result = false;
			return std::nullopt;
		}

		if (!result && extractWhat) {
			SPDLOG_ERROR("No string has been extracted from {} by regexp {}", extractFrom, usedRegex);
			return std::nullopt;
		}
	}


	if (result) {
		return std::optional<std::string>(match[0].str());
	}
	else {
		return std::nullopt;
	}

}

/**
 * Checks airspace reservations against the configuration to verify
 * if there is anything, what will be said taking into acount which
 * types of airspace are enabled
 * @param airspaceReservations
 * @param config
 * @return
 */
bool PlaylistAssemblerAirspace::checkIfThereIsAnythingToSayAroundPoint(
		const std::map<std::string, PansaAirspace_Zone> &airspaceReservations,
		const ConfigurationFile_Airspace_SayConfigPerElemType &config,
		const std::vector<std::string> & filter,
		const std::vector<std::string> & designatorsAlreadyAdded){

	bool output = false;

	// iterator to reservations map
	std::map<std::string, PansaAirspace_Zone>::const_iterator it = airspaceReservations.begin();

	// iterate through all airspace, which has any reservation
	do {

		const PansaAirspace_Type& t = it->second.type;
		const std::string designator = it->first;

		// check if this type of airspace is enabled by configuration
		const bool sayThis = PlaylistAssemblerAirspace::checkIfAirspaceTypeEnabled(t, config);

		// go through configure filters and look for
		const bool isFiltered = PlaylistAssemblerAirspace::checkIfFiltered(designator, filter);

		const auto alreadyAdded = std::find(designatorsAlreadyAdded.begin(), designatorsAlreadyAdded.end(), designator);

		if (!isFiltered) {
			if (alreadyAdded == designatorsAlreadyAdded.end()) {
				if (sayThis) {
					output = true;
					break;
				}
			}
			else {
				SPDLOG_INFO("Anouncement for {} has been added before as a part of explicitly configured airspace", designator);
			}
		}
		else {
			SPDLOG_INFO("Airspace designator {} is filtered and won't be added to playlist", designator);
		}
	} while (++it != airspaceReservations.end());

	return output;
}

/**
 * Puts all announcement elements, which are common for both Explicitly configured airspaces
 * and reservation / restriction around given point. These are airspace designator and
 * optionally altitude and time range
 * @param designatorsAnouncementsDict dictionary with custom announcements for certain airspaces
 * @param airspaceCfg global configuration red from tag 'Airspace' in config file
 * @param airspaceFullDesignatorString like "ATZ EPBA" or "EPTR03C", which might have some unwanted characters
 * @param type airspace type like ATZ, TRA, TSA and so on
 * @param reservations vector of all active reservation downloaded from the API, which shall be put into anouncement
 * @param sayAltitudeRangeOfReservations if an announcement should contains altitude range for each reservation
 * @param sayTimeRangeOfReservations if an announcement should contains time (as hours) range for each reservation
 */
int PlaylistAssemblerAirspace::insertCommonAnnouncementAudioElems(
		const std::map<std::string, std::string> & designatorsAnouncementsDict,
		const ConfigurationFile_Airspace & airspaceCfg,
		const std::string & airspaceFullDesignatorString,
		const PansaAirspace_Type & type,
		const std::vector<std::shared_ptr<PansaAirspace_Reservation>> & reservations,
		bool sayAltitudeRangeOfReservations,
		bool sayTimeRangeOfReservations) {

	int out = 0;

	const std::shared_ptr<std::vector<std::string>> playlistPtr = playlist.value();

	bool atLeastOneFitIntoTimeframe = false;

	// checks if at least one reservation fits within future time margin AND
	// it hasn't finished yet (so it is currently active)
	for (const std::shared_ptr<PansaAirspace_Reservation> rsvr : reservations) {
		// check if this reservation is in the past
		if (!airspaceCfg.sayPast && ((uint64_t)TimeTools::getEpoch() > rsvr->toTime)) {
			// this is too old, go the next one
			continue;
		}

		if (airspaceCfg.reservationFutureTimeMargin != 0) {
			if ((uint64_t)(TimeTools::getEpoch() + airspaceCfg.reservationFutureTimeMargin * 60ULL) < rsvr->fromTime) {
				// this will start to far in the future
				continue;
			}
		}

		atLeastOneFitIntoTimeframe = true;
		break;
	}

	// check if any reservation start and end time
	if (!atLeastOneFitIntoTimeframe) {
		SPDLOG_WARN("There are no reservations for {} with active time matching announcement configuration.", airspaceFullDesignatorString);
	}
	else {

		// check if there is an explicit audio file for this designator
		const std::map<std::string, std::string>::const_iterator itAnnouncement = designatorsAnouncementsDict.find(airspaceFullDesignatorString);

		if (itAnnouncement != designatorsAnouncementsDict.end()) {
			// there is an announcement from audio file
			playlistPtr->push_back(itAnnouncement->second);
		}
		else {
			// generate generic one

			// add airspace type announcement
			playlistPtr->push_back(sampler->getForAirspaceType(type));

			if (airspaceCfg.genericAnouncementFromRegex) {
				// extract designator string
				const std::optional<std::string> designatorStr = PlaylistAssemblerAirspace::extractUsingRegexp(airspaceFullDesignatorString, true, type, airspaceCfg.confPerElemType);

				// extract (optional) sector letter
				const std::optional<std::string> sectorStr = PlaylistAssemblerAirspace::extractUsingRegexp(airspaceFullDesignatorString, false, type, airspaceCfg.confPerElemType);

				if (!designatorStr.has_value()) {
					throw std::runtime_error("Designator for airspace must be extracted");
				}
				else {
					SPDLOG_DEBUG("Designator {} extracted from full-designator-string for airspace {}", designatorStr.value(), airspaceFullDesignatorString);
				}

				// say designator in phonetical way. like: "Echo Papa Bravo Alpha"
				std::vector<std::string> designatorAudio = sampler->getPhoneticForWord(designatorStr.value());
				playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(designatorAudio.begin()), std::make_move_iterator(designatorAudio.end()));

				// optionally add sector announcement
				if (sectorStr.has_value()) {
					// special check for ATZ. usually is not sectorized although, but if it its designator may looks like: "ATZ EPGLA"
					if (type == AIRSPACE_ATZ) {
						// check if the designator is five characters long, what suggests that it has a sector at the end
						std::regex lenghtTest = std::regex("[A-Z]{5}$", std::regex::ECMAScript);

					    std::smatch lenghTestMatch;

						// try to extract last 5 characters from airspace designator
					    const bool result = std::regex_search(airspaceFullDesignatorString, lenghTestMatch, lenghtTest);

					    // if it was extraced
					    if (result && lenghTestMatch[0].length() == 5) {
							playlistPtr->push_back(sampler->getAirspaceConstantElement(PlaylistSampler_Airspace::AIRSPACE_SECTOR));

							SPDLOG_DEBUG("Sector {} extracted from full-designator-string for ATZ {}", sectorStr.value(), airspaceFullDesignatorString);
							std::vector<std::string> sectorAudio = sampler->getPhoneticForWord(sectorStr.value());
							playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(sectorAudio.begin()), std::make_move_iterator(sectorAudio.end()));
					    }
					}
					else {
						playlistPtr->push_back(sampler->getAirspaceConstantElement(PlaylistSampler_Airspace::AIRSPACE_SECTOR));

						SPDLOG_DEBUG("Sector {} extracted from full-designator-string for airspace {}", sectorStr.value(), airspaceFullDesignatorString);
						std::vector<std::string> sectorAudio = sampler->getPhoneticForWord(sectorStr.value());
						playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(sectorAudio.begin()), std::make_move_iterator(sectorAudio.end()));
					}

				}
			}
			else {
				// splitted designator
				std::vector<std::string> words;

				// final string to be converted to voice announcement
				std::string out;

				// count words in designator
				boost::split(words, airspaceFullDesignatorString, boost::is_space(), boost::token_compress_off);

				if (airspaceCfg.glueGenericAnouncement) {
					out = boost::algorithm::join(words, "");
				}
				else {
					// use first word from
					out = words[0];
				}

				std::vector<std::string> designatorAudio = sampler->getPhoneticForWord(out);
				playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(designatorAudio.begin()), std::make_move_iterator(designatorAudio.end()));
			}

		}

		// "active"
		playlistPtr->push_back(sampler->getAirspaceConstantElement(PlaylistSampler_Airspace::AIRSPACE_ACTIVE));

		// iterate through all reservations
		for (const std::shared_ptr<PansaAirspace_Reservation> & ptr: reservations) {
			// reservation held by this pointer
			const PansaAirspace_Reservation r = *ptr;

			// check if this reservation is in the past
			if (!airspaceCfg.sayPast && ((uint64_t)TimeTools::getEpoch() > r.toTime)) {
				SPDLOG_INFO("Reservation made by {} ommitted as there is {} minutes after it had ended", r.unit, (TimeTools::getEpoch() - r.toTime) / 60);
				continue;
			}

			if (airspaceCfg.reservationFutureTimeMargin != 0) {
				if ((uint64_t)(TimeTools::getEpoch() + airspaceCfg.reservationFutureTimeMargin * 60ULL) < r.fromTime) {
					SPDLOG_INFO("Reservation made by {} ommitted as it starts {} minutes, or {} hours in the future", r.unit, (r.fromTime - (uint64_t)TimeTools::getEpoch()) / 60ULL, (r.fromTime - (uint64_t)TimeTools::getEpoch()) / 3600ULL);
					SPDLOG_INFO("Future time limit is set to {} minutes", airspaceCfg.reservationFutureTimeMargin);
					continue;
				}
			}

			if (sayTimeRangeOfReservations) {
				// from time
				playlistPtr->push_back(sampler->getConstantElement(PlaylistSampler_ConstanElement::FROM).value());
				std::vector<std::string> timeFrom = sampler->getAudioForHourAndMinute(r.fromTime, true);
				playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(timeFrom.begin()), std::make_move_iterator(timeFrom.end()));

				// to time
				playlistPtr->push_back(sampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
				timeFrom = sampler->getAudioForHourAndMinute(r.toTime, true);
				playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(timeFrom.begin()), std::make_move_iterator(timeFrom.end()));
			}

			// check if altitude shall be included
			if (sayAltitudeRangeOfReservations) {

				// "altitude from"
				playlistPtr->push_back(sampler->getAirspaceConstantElement(PlaylistSampler_Airspace::AIRSPACE_ALTITUDE));
				playlistPtr->push_back(sampler->getConstantElement(PlaylistSampler_ConstanElement::FROM).value());

				if (r.lowerAltitude > 0)  {
					// "XXXX meters"
					auto fromMeters = sampler->getAudioListFromNumber(PlaylistAssemblerAirspace::convertMetersToHundretsMeters(r.lowerAltitude, false));
					playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(fromMeters.begin()), std::make_move_iterator(fromMeters.end()));
					playlistPtr->push_back(sampler->getAudioFromUnit(PlaylistSampler_Unit::METER, r.lowerAltitude));
				}
				else {
					// ground
					playlistPtr->push_back(sampler->getAirspaceConstantElement(PlaylistSampler_Airspace::AIRSPACE_GROUND));
				}

				// "to"
				playlistPtr->push_back(sampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());

				// "XXXX meters"
				auto toMeters = sampler->getAudioListFromNumber(PlaylistAssemblerAirspace::convertMetersToHundretsMeters(r.upperAltitude, false));
				playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(toMeters.begin()), std::make_move_iterator(toMeters.end()));
				playlistPtr->push_back(sampler->getAudioFromUnit(PlaylistSampler_Unit::METER, r.upperAltitude));


			}
			out++;		// increase number of anouncements added
		}
	}

	return out;
}

/**
 * Recalculates altitude from any value between 100 to 9999 meters to 100 meters increments
 * @param altitude in meters
 * @param trueToRoundUp set to true to round altitude to next 100 increment, like 1822 meters to 1900 meters
 * @return
 */
int PlaylistAssemblerAirspace::convertMetersToHundretsMeters(int altitude,
		bool trueToRoundUp) {

	int out = 0;

	if (altitude > 0) {
		const int amountOfHundrets = (altitude / 100);

		out = amountOfHundrets * 100;

		if (trueToRoundUp && ((altitude % 100) != 0)) {
			out += 100;
		}

	}

	return out;
}

/**
 * Inserts single announcement for an airspace explicitly configured and checked in the API
 * @param airspace configuration of this airspace
 * @param reservations result of a query, the vector may be empty if no reservations are active currently
 */
void PlaylistAssemblerAirspace::reservationsForExplicitlyConfAirspace(
		const std::string designator,
		const bool sayAltitudes,
		const bool sayTimes,
		const PansaAirspace_Type type,
		const std::vector<std::shared_ptr<PansaAirspace_Reservation>> & activeReservations) {

	// check if there are any reservations
	if (activeReservations.size() == 0) {
		SPDLOG_WARN("There are no reservations for airspace {}", designator);
		return;
	}

	const std::shared_ptr<std::vector<std::string>> playlistPtr = playlist.value();
	const ConfigurationFile_Airspace & airspaceCfg = config->getAirspace();
	const std::map<std::string, std::string> & designatorsAnouncementsDict = airspaceCfg.airspaceDesignatorsAnouncement;

	// combination of global and per airspace setting for "should I say any altitude range for this"
	bool localSayAltitudes = config->getAirspace().sayAltitudes;

	// if say altitudes is enabled globaly
	if (localSayAltitudes) {
		// enable or disable it basing on configuration specific for that
		localSayAltitudes = sayAltitudes;
	}
	else {
		; // if it is not enabled globally, ignore what is set specific here
	}

	// check if this designator has been already added as a element of
	// another type of anouncement (like designator around the point)
	const auto alreadyAdded = std::find(this->designatorsAlreadyAdded.begin(), this->designatorsAlreadyAdded.end(), designator);

	if (alreadyAdded != this->designatorsAlreadyAdded.end()) {
		// this has been added
		SPDLOG_INFO("Anouncement for {} is skipped because it has been added as a part of restriction around point", designator);
		return;
	}

	SPDLOG_INFO("Adding anouncement for explicitly configured airspace designator {}", designator);

	int anouncements = insertCommonAnnouncementAudioElems(designatorsAnouncementsDict, airspaceCfg, designator, type, activeReservations, localSayAltitudes, sayTimes);

	this->designatorsAlreadyAdded.push_back(designator);

	if (anouncements > 0) {
		this->hasSomethingToSay = true;
	}

}

PlaylistAssemblerAirspace::~PlaylistAssemblerAirspace() {
	// TODO Auto-generated destructor stub
}

void PlaylistAssemblerAirspace::reservationsAroundPoint(
		const int radiusInMeters,
		const std::string anouncementAudioFilename,
		const std::map<std::string, PansaAirspace_Zone>& airspaceReservations) {

	if (!playlist.has_value()) {
		SPDLOG_ERROR("Existing playlist hasn't been set!");
		return;
	}

	if (airspaceReservations.size() == 0) {
		SPDLOG_WARN("Empty airspace reservations map was provided");
		return;
	}

	SPDLOG_INFO("Adding reservations aroung {} meters around {}", radiusInMeters, anouncementAudioFilename);

	const std::shared_ptr<std::vector<std::string>> playlistPtr = playlist.value();
	const ConfigurationFile_Airspace & airspaceCfg = config->getAirspace();
	const std::map<std::string, std::string> & designatorsAnouncementsDict = airspaceCfg.airspaceDesignatorsAnouncement;

	const bool anythingToSay = PlaylistAssemblerAirspace::checkIfThereIsAnythingToSayAroundPoint(airspaceReservations, airspaceCfg.confPerElemType, airspaceCfg.designatorsFilter, this->designatorsAlreadyAdded);

	if (!anythingToSay) {
		SPDLOG_INFO("There is nothing to say around this point, because types of all active airspaces are disabled by config");
		return;
	}

	int radiusKm = (int)::round(radiusInMeters / 1000);

	// ograniczenia lotów w promieniu xx kilometrów od lokalizacji skrzyczne
	// .... strefa ograniczeń Romeo jeden dwa trzy sektor bravo

	// "flight restriction"
	playlistPtr->push_back(sampler->getAirspaceConstantElement(PlaylistSampler_Airspace::AIRSPACE_RESTRICTIONS_IN));

	// "in radius of"
	playlistPtr->push_back(sampler->getConstantElement(PlaylistSampler_ConstanElement::IN_RADIUS).value());

	// "XX kilometers"
	auto radius = sampler->getAudioListFromNumber(radiusKm);
	playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(radius.begin()), std::make_move_iterator(radius.end()));
	playlistPtr->push_back(sampler->getAudioFromUnit(PlaylistSampler_Unit::KM, radiusKm));

	// "from location"
	playlistPtr->push_back(sampler->getConstantElement(PlaylistSampler_ConstanElement::FROM_LOCATION).value());

	// location itself
	playlistPtr->push_back(anouncementAudioFilename);

	// iterator to reservations map
	std::map<std::string, PansaAirspace_Zone>::const_iterator it = airspaceReservations.begin();

	// iterate through all airspace, which has any reservation
	do {

		// get full-designator-string
		const std::string fullDesignatorString = it->first;

		// get airspace type
		const PansaAirspace_Type type = it->second.type;

		// vector of all active reservations for this airspace, which have been fetched from Pansa API
		const std::vector<std::shared_ptr<PansaAirspace_Reservation>> & activeReservations = it->second.reservations;

		// check if it filtered out TODO:
		const bool filtered = PlaylistAssemblerAirspace::checkIfFiltered(fullDesignatorString, airspaceCfg.designatorsFilter);

		// check if announcement for this airspace type is enabled
		const bool typeEnabled = PlaylistAssemblerAirspace::checkIfAirspaceTypeEnabled(type, airspaceCfg.confPerElemType);

		if (filtered) {
			SPDLOG_INFO("{} has been filtered out and will be skipped", it->first);
			continue;
		}

		// check if this designator has been already added as a element of
		// another type of anouncement (like explicitly configured airspace)
		const auto alreadyAdded = std::find(this->designatorsAlreadyAdded.begin(), this->designatorsAlreadyAdded.end(), fullDesignatorString);

		if (alreadyAdded != this->designatorsAlreadyAdded.end()) {
			// this has been added
			SPDLOG_INFO("Anouncement for {} is skipped because it has been added as explicitly configured airspace", fullDesignatorString);
			continue;
		}

		// skip this reservation if this is not enabled
		if (!typeEnabled) {
			SPDLOG_DEBUG("Skipping {} because airspace type {} is disabled by config", it->first, PansaAirspace_Type_ToString(type));
			continue;
		}
		else {
			SPDLOG_INFO("Addind airspace {} of type {} with {} reservations", it->first, PansaAirspace_Type_ToString(type), activeReservations.size());
			this->designatorsAlreadyAdded.push_back(fullDesignatorString);
		}

		//
		int anouncements = insertCommonAnnouncementAudioElems(designatorsAnouncementsDict, airspaceCfg, fullDesignatorString, type, activeReservations, config->getAirspace().sayAltitudes, true);

		if (anouncements > 0) {
			this->hasSomethingToSay = true;
		}

	} while (++it != airspaceReservations.end());

}

#endif

/*
 * PlaylistAssemblerAirspace.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: mateusz
 */

#include "PlaylistAssemblerAirspace.h"

#ifdef PANSA_AIRSPACE

#include <regex>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")


PlaylistAssemblerAirspace::PlaylistAssemblerAirspace(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config) :
																sampler(sampler), config(config) {
	// TODO Auto-generated constructor stub

}

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
 * @param extractFrom full-designator-string to extract from
 * @param extractWhat true to extract airspace designator or false to extract sector (if applicable)
 * @param type
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
				;
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

PlaylistAssemblerAirspace::~PlaylistAssemblerAirspace() {
	// TODO Auto-generated destructor stub
}

void PlaylistAssemblerAirspace::reservationsAroundPoint(
		const ConfigurationFile_Airspace_AroundPoint & point,
		const std::map<std::string, PansaAirspace_Zone>& airspaceReservations) {

	if (!playlist.has_value()) {
		SPDLOG_ERROR("Existing playlist hasn't been set!");
		return;
	}

	if (airspaceReservations.size() == 0) {
		SPDLOG_WARN("Empty airspace reservations map was provided");
		return;
	}

	const std::shared_ptr<std::vector<std::string>> playlistPtr = playlist.value();
	const ConfigurationFile_Airspace & airspaceCfg = config->getAirspace();
	const std::map<std::string, std::string> & designatorsAnouncementsDict = airspaceCfg.airspaceDesignatorsAnouncement;

	int radiusKm = (int)::round(point.radius / 1000);

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
	playlistPtr->push_back(point.audioFilename);

	// iterator to reservations map
	std::map<std::string, PansaAirspace_Zone>::const_iterator it = airspaceReservations.begin();

	// iterate through all airspace, which has any reservation
	do {

		// get airspace type
		const PansaAirspace_Type type = it->second.type;

		// check if announcement for this airspace type is enabled
		const bool typeEnabled = PlaylistAssemblerAirspace::checkIfAirspaceTypeEnabled(type, airspaceCfg.confPerElemType);

		// skip this reservation if this is not enabled
		if (!typeEnabled) {
			SPDLOG_DEBUG("Skipping {} because airspace type {} is disabled by config", it->first, PansaAirspace_Type_ToString(type));
			continue;
		}
		else {
			SPDLOG_INFO("Addind airspace {} of type {} with {} reservations", it->first, PansaAirspace_Type_ToString(type), it->second.reservations.size());
		}

		// check if there is an explicit announcement for this designator
		const std::map<std::string, std::string>::const_iterator itAnnouncement = designatorsAnouncementsDict.find(it->first);

		if (itAnnouncement != designatorsAnouncementsDict.end()) {
			// there is an announcement
			playlistPtr->push_back(itAnnouncement->second);
		}
		else {
			// generate generic one

			// add airspace type announcement
			playlistPtr->push_back(sampler->getForAirspaceType(type));

			// extract designator string
			const std::optional<std::string> designatorStr = PlaylistAssemblerAirspace::extractUsingRegexp(it->first, true, type, airspaceCfg.confPerElemType);

			// extract (optional) sector letter
			const std::optional<std::string> sectorStr = PlaylistAssemblerAirspace::extractUsingRegexp(it->first, false, type, airspaceCfg.confPerElemType);

			if (!designatorStr.has_value()) {
				throw std::runtime_error("Designator for airspace must be extracted");
			}
			else {
				SPDLOG_DEBUG("Designator {} extracted from full-designator-string for airspace {}", designatorStr.value(), it->first);
			}

			std::vector<std::string> designatorAudio = sampler->getPhoneticForWord(designatorStr.value());
			playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(designatorAudio.begin()), std::make_move_iterator(designatorAudio.end()));

			// optionally add sector anouncement
			if (sectorStr.has_value()) {
				SPDLOG_DEBUG("Sector {} extracted from full-designator-string for airspace {}", sectorStr.value(), it->first);
				std::vector<std::string> sectorAudio = sampler->getPhoneticForWord(sectorStr.value());
				playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(sectorAudio.begin()), std::make_move_iterator(sectorAudio.end()));
			}

		}

	} while (++it != airspaceReservations.end());

}

#endif

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

PlaylistAssemblerAirspace::~PlaylistAssemblerAirspace() {
	// TODO Auto-generated destructor stub
}

void PlaylistAssemblerAirspace::reservationsAroundPoint(
		ConfigurationFile_Airspace_AroundPoint & point,
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

	// ograniczenia lotów w promieniu xx kilometrów od lokalizacji skrzyczne
	// .... strefa ograniczeń Romeo jeden dwa trzy sektor bravo

	// "flight restriction"
	playlistPtr->push_back(sampler->getAirspaceConstantElement(PlaylistSampler_Airspace::AIRSPACE_RESTRICTIONS_IN));

	// "in radius of"
	playlistPtr->push_back(sampler->getConstantElement(PlaylistSampler_ConstanElement::IN_RADIUS).value());

	// "XX kilometers"
	auto radius = sampler->getAudioListFromNumber(point.radius);
	playlistPtr->insert(playlistPtr->end(), std::make_move_iterator(radius.begin()), std::make_move_iterator(radius.end()));
	playlistPtr->push_back(sampler->getAudioFromUnit(PlaylistSampler_Unit::KM, point.radius));

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

			// result of matching designator from full-designator-string
			bool designatorResult = false;

			// add airspace type announcement
			playlistPtr->push_back(sampler->getForAirspaceType(type));

			// regular expression used to extract designator and (optional) sector from full-designator-string
			std::regex designator;
			std::regex sector;

			// results of matching designator and (optional) sector from full-designator-string
		    std::smatch designatorMatch;
		    std::smatch sectorMatch;

			// extract designator and optional sector information
			switch (type) {
				case AIRSPACE_TRA:
					designator = std::regex(airspaceCfg.confPerElemType.traDesignatorRegexp, std::regex::grep);
					sector = std::regex(airspaceCfg.confPerElemType.traSectorRegexp, std::regex::grep);

					designatorResult = 	std::regex_search(it->first, designatorMatch, designator);
										std::regex_search(it->first, sectorMatch, sector);

					if (!designatorResult) {
						SPDLOG_ERROR("No designator string has been extracted from {} by regexp {}", it->first, airspaceCfg.confPerElemType.traDesignatorRegexp);
						throw std::runtime_error("");
					}

					break;
				case AIRSPACE_TSA:
					designator = std::regex(airspaceCfg.confPerElemType.tsaDesignatorRegexp, std::regex::grep);
					sector = std::regex(airspaceCfg.confPerElemType.tsaSectorRegexp, std::regex::grep);

					designatorResult = 	std::regex_search(it->first, designatorMatch, designator);
										std::regex_search(it->first, sectorMatch, sector);

					if (!designatorResult) {
						SPDLOG_ERROR("No designator string has been extracted from {} by regexp {}", it->first, airspaceCfg.confPerElemType.tsaDesignatorRegexp);
						throw std::runtime_error("");
					}

					break;
				case AIRSPACE_ATZ:
					designator = std::regex(airspaceCfg.confPerElemType.atzDesignatorRegexp, std::regex::grep);
					designatorResult = 	std::regex_search(it->first, designatorMatch, designator);

					if (!designatorResult) {
						SPDLOG_ERROR("No designator string has been extracted from {} by regexp {}", it->first, airspaceCfg.confPerElemType.atzDesignatorRegexp);
						throw std::runtime_error("");
					}

					break;
				case AIRSPACE_D:
					designator = std::regex(airspaceCfg.confPerElemType.dDesignatorRegexp, std::regex::grep);
					sector = std::regex(airspaceCfg.confPerElemType.dSectorRegexp, std::regex::grep);

					designatorResult = 	std::regex_search(it->first, designatorMatch, designator);
										std::regex_search(it->first, sectorMatch, sector);

					if (!designatorResult) {
						SPDLOG_ERROR("No designator string has been extracted from {} by regexp {}", it->first, airspaceCfg.confPerElemType.dDesignatorRegexp);
						throw std::runtime_error("");
					}

					break;
				case AIRSPACE_R:
					designator = std::regex(airspaceCfg.confPerElemType.rDesignatorRegexp, std::regex::grep);
					designatorResult = 	std::regex_search(it->first, designatorMatch, designator);

					if (!designatorResult) {
						SPDLOG_ERROR("No designator string has been extracted from {} by regexp {}", it->first, airspaceCfg.confPerElemType.rDesignatorRegexp);
						throw std::runtime_error("");
					}

					break;
				case AIRSPACE_P: break;
				case AIRSPACE_ADHOC:
					designator = std::regex(airspaceCfg.confPerElemType.tsaDesignatorRegexp, std::regex::grep);
					sector = std::regex(airspaceCfg.confPerElemType.tsaSectorRegexp, std::regex::grep);

					designatorResult = 	std::regex_search(it->first, designatorMatch, designator);
										std::regex_search(it->first, sectorMatch, sector);

					if (!designatorResult) {
						SPDLOG_ERROR("No designator string has been extracted from {} by regexp {}", it->first, airspaceCfg.confPerElemType.tsaDesignatorRegexp);
						throw std::runtime_error("");
					}

					break;
			}

			const std::string designatorStr = designatorMatch[0].str();
			const std::string sectorStr = sectorMatch[0].str();


		}

	} while (it++ != airspaceReservations.end());

}

#endif

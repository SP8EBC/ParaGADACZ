/*
 * PlaylistAssemblerAirspace.h
 *
 *  Created on: Jan 17, 2024
 *      Author: mateusz
 */

#ifndef PLAYLISTASSEMBLERAIRSPACE_H_
#define PLAYLISTASSEMBLERAIRSPACE_H_

#ifdef PANSA_AIRSPACE_ENABLED

#include <memory>
#include <optional>

#include "ConfigurationFile.h"
#include "PlaylistSampler.h"
#include "PansaAirspace.h"


class PlaylistAssemblerAirspace {
private:

	bool hasSomethingToSay;

	std::optional<std::shared_ptr<std::vector<std::string>>> playlist;

	std::shared_ptr<PlaylistSampler> & sampler;

	std::shared_ptr<ConfigurationFile> & config;

	/**
	 * Vector of all designators, which were already added to a playlist as
	 * airspaces around a point. This is not to double the same designator
	 * in explicitly designated section
	 */
	std::vector<std::string> designatorsAlreadyAdded;

	/**
	 * Checks airspace reservations against the configuration to verify
	 * if there is anything, what will be said taking into acount which
	 * types of airspace are enabled
	 * @param airspaceReservations map between airspace designator (like ATZ EPBA) and reservations + type + centroid locations + designator once again
	 * @param config
	 * @return
	 */
	static bool checkIfThereIsAnythingToSayAroundPoint(
										const std::map<std::string, PansaAirspace_Zone>& airspaceReservations,
										const ConfigurationFile_Airspace_SayConfigPerElemType &config,
										const std::vector<std::string> & filter,
										const std::vector<std::string> & designatorsAlreadyAdded,
										const std::string & point);

	/**
	 * Checks if a kind of airspace (like TRA, TSA etc) is currently enabled by user configuration
	 * @param type airspace type
	 * @param config parsed from configuration file
	 * @return
	 */
	static bool checkIfAirspaceTypeEnabled(PansaAirspace_Type type, const ConfigurationFile_Airspace_SayConfigPerElemType & config);

	/**
	 *
	 * @param designator
	 * @param filter
	 * @return
	 */
	static bool checkIfFiltered(const std::string & designator, const std::vector<std::string> & filter);

	/**
	 *
	 * @param extractFrom full-designator-string to extract from
	 * @param extractWhat true to extract airspace designator or false to extract sector (if applicable)
	 * @param type
	 * @param confPerElemType
	 * @return
	 */
	static std::optional<std::string> extractUsingRegexp(
									std::string extractFrom,
									bool extractWhat,
									const PansaAirspace_Type type,
									const ConfigurationFile_Airspace_SayConfigPerElemType & confPerElemType);

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
	int insertCommonAnnouncementAudioElems(
								const std::map<std::string, std::string> & designatorsAnouncementsDict,
								const ConfigurationFile_Airspace & airspaceCfg,
								const std::string & airspaceFullDesignatorString,
								const PansaAirspace_Type & type,
								const std::vector<std::shared_ptr<PansaAirspace_Reservation>> & reservations,
								bool sayAltitudeRangeOfReservations,
								bool sayTimeRangeOfReservations);

protected:
	/**
	 * Recalculates altitude from any value between 100 to 9999 meters to 100 meters increments
	 * @param altitude in meters
	 * @param trueToRoundUp set to true to round altitude to next 100 increment, like 1822 meters to 1900 meters
	 * @return
	 */
	static int convertMetersToHundretsMeters(int altitude, bool trueToRoundUp);

public:

	/**
	 * Inserts announcements for all reservations found around the point
	 * @param point
	 * @param airspaceReservations map between airspace designator (like ATZ EPBA) and reservations + type + centroid locations + designator once again
	 */
	void reservationsAroundPoint(
					const int radiusInMeters,
					const std::string anouncementAudioFilename,
					const std::map<std::string, PansaAirspace_Zone>& airspaceReservations);

	/**
	 * Inserts single announcement for an airspace explicitly configured and checked in the API
	 * @param airspace configuration of this airspace
	 * @param reservations result of a query, the vector may be empty if no reservations are active currently
	 */
	void reservationsForExplicitlyConfAirspace(
			const std::string designator,
			const bool sayAltitudes,
			const bool sayTimes,
			const PansaAirspace_Type type,
			const std::vector<std::shared_ptr<PansaAirspace_Reservation>> & activeReservations);

	PlaylistAssemblerAirspace(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config);
	virtual ~PlaylistAssemblerAirspace();

	const std::shared_ptr<std::vector<std::string> > & getPlaylist() const {
		return playlist.value();
	}

	void setPlaylist(
			const std::optional<std::shared_ptr<std::vector<std::string> > > &playlist) {
		this->playlist = playlist;
	}

	bool isHasSomethingToSay() const {
		return hasSomethingToSay;
	}
};

#endif

#endif /* PLAYLISTASSEMBLERAIRSPACE_H_ */

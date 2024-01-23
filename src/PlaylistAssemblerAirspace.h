/*
 * PlaylistAssemblerAirspace.h
 *
 *  Created on: Jan 17, 2024
 *      Author: mateusz
 */

#ifndef PLAYLISTASSEMBLERAIRSPACE_H_
#define PLAYLISTASSEMBLERAIRSPACE_H_

#ifdef PANSA_AIRSPACE

#include <memory>
#include <optional>

#include "ConfigurationFile.h"
#include "PlaylistSampler.h"
#include "PansaAirspace.h"


class PlaylistAssemblerAirspace {
private:
	std::optional<std::shared_ptr<std::vector<std::string>>> playlist;

	std::shared_ptr<PlaylistSampler> & sampler;

	std::shared_ptr<ConfigurationFile> & config;

	static bool checkIfAirspaceTypeEnabled(PansaAirspace_Type type, const ConfigurationFile_Airspace_SayConfigPerElemType & config);
public:

	/**
	 * Inserts announcements for all reservations found around the point
	 * @param point
	 * @param airspaceReservations map between airspace designator (like ATZ EPBA) and reservations + type + centroid locations + designator once again
	 */
	void reservationsAroundPoint(ConfigurationFile_Airspace_AroundPoint & point, const std::map<std::string, PansaAirspace_Zone>& airspaceReservations);

	PlaylistAssemblerAirspace(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config);
	virtual ~PlaylistAssemblerAirspace();

	const std::shared_ptr<std::vector<std::string> > & getPlaylist() const {
		return playlist.value();
	}

	void setPlaylist(
			const std::optional<std::shared_ptr<std::vector<std::string> > > &playlist) {
		this->playlist = playlist;
	}
};

#endif

#endif /* PLAYLISTASSEMBLERAIRSPACE_H_ */

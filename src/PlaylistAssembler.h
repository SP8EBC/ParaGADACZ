/*
 * PlaylistAssembler.h
 *
 *	This class takes all numeric data and then assemble complete playlist using given playlist sampler
 *
 *  Created on: Jan 11, 2023
 *      Author: mateusz
 */

#ifndef PLAYLISTASSEMBLER_H_
#define PLAYLISTASSEMBLER_H_

#include <string>
#include "PlaylistSampler.h"
#include "Inline_response_200.h"
#include "ConfigurationFile.h"
#include "StationApi.h"
#include "AprsWXData.h"

class PlaylistAssembler {

	PlaylistSampler & playlistSampler;

public:
	PlaylistAssembler(PlaylistSampler & sampler, std::string audioBasePath);
	virtual ~PlaylistAssembler();
	PlaylistAssembler(const PlaylistAssembler &other);
	PlaylistAssembler(PlaylistAssembler &&other);
	PlaylistAssembler& operator=(const PlaylistAssembler &other);
	PlaylistAssembler& operator=(PlaylistAssembler &&other);

	/**
	 * Initializes new playlist and appends an intro to it and optionally current time
	 *
	 * \param currentTime if current time announcement should be added to playlist
	 */
	void start(bool currentTime);

	/**
	 * Appends an announcement with regional pressure
	 *
	 * \param pressure Pressure value
	 */
	void regionalPressure (float pressure);

	/**
	 * Appends current weather conditions. Internally this methof works
	 */
	void currentWeather(ConfigurationFile & config, std::vector<org::openapitools::client::model::Summary> & summary, std::vector<AprsWXData> & result);
};

#endif /* PLAYLISTASSEMBLER_H_ */

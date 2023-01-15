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
#include <utility>
#include "PlaylistSampler.h"
#include "Inline_response_200.h"
#include "ConfigurationFile.h"
#include "StationApi.h"
#include "AprsWXData.h"

class PlaylistAssembler {

	std::vector<std::string> playlist;

	PlaylistSampler & playlistSampler;

	ConfigurationFile & configurationFile;

public:
	PlaylistAssembler(PlaylistSampler & sampler, ConfigurationFile & config);
	virtual ~PlaylistAssembler();
	PlaylistAssembler(const PlaylistAssembler &other);
	PlaylistAssembler(PlaylistAssembler &&other);
	PlaylistAssembler& operator=(const PlaylistAssembler &other);
	PlaylistAssembler& operator=(PlaylistAssembler &&other);

	/**
	 * Initializes new playlist and appends an intro to it and optionally current time
	 */
	void start();

	/**
	 * Appends an announcement with regional pressure
	 *
	 * \param pressure Pressure value
	 */
	void regionalPressure (float pressure);

	/**
	 * Appends current weather conditions. It maintains an order defined in configuration file
	 *
	 * \param summary Vector of pairs with station_name - station_summary
	 */
	void currentWeather(
			std::vector<std::pair<std::string, org::openapitools::client::model::Summary>> & summary,
			std::vector<AprsWXData> & result);

	/**
	 * Appends meteoblue forecast
	 */
	void forecastMeteoblue(std::vector<org::openapitools::client::model::Inline_response_200> forecasts);

	/**
	 * Appends prerecorded anouncement either at the begining or the end
	 */
	void recordedAnnouncement(bool preOrPost);

	/**
	 * Appends sign off message
	 */
	void signOff();

	const std::vector<std::string> & getPlaylist() {
		return playlist;
	}
};

#endif /* PLAYLISTASSEMBLER_H_ */

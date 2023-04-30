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
#include <memory>

#include "PlaylistSampler.h"
#include "Inline_response_200.h"
#include "ConfigurationFile.h"
#include "StationApi.h"
#include "AprsWXData.h"
#include "AvalancheWarnings.h"

class PlaylistAssembler {

	std::shared_ptr<std::vector<std::string>> playlist;

	std::shared_ptr<PlaylistSampler> & playlistSampler;

	std::shared_ptr<ConfigurationFile> & configurationFile;

	static std::string throwOnEmptyOptional();

private:

	/**
	 * Helper private method for assembling rain forecast
	 * @param forecast
	 */
	void putRainForecastFromMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> & forecast);

public:
	PlaylistAssembler(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config);
	virtual ~PlaylistAssembler();
	PlaylistAssembler(const PlaylistAssembler &other);
	PlaylistAssembler(PlaylistAssembler &&other);

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
			std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> & summary,
			std::vector<AprsWXData> & result,
			std::vector<std::tuple<std::string, AprsWXData>> & weatherlink);

	/**
	 * Appends meteoblue forecast
	 */
	void forecastMeteoblue(std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> & forecasts);

	/**
	 * Avalanche warnings
	 */
	void avalancheWarning(AvalancheWarnings_Location location, int8_t level, AvalancheWarnings_Expositions expositions);

	/**
	 * Appends prerecorded anouncement either at the begining or the end
	 */
	void recordedAnnouncement(bool preOrPost);

	/**
	 * Appends sign off message
	 */
	void signOff();

	const std::shared_ptr<std::vector<std::string>> getPlaylist() {
		return playlist;
	}
};

#endif /* PLAYLISTASSEMBLER_H_ */

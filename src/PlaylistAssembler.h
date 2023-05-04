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

protected:

	/**
	 *	Struct to sum different kinds of meteoblue rainspots
	 */
	struct PlaylistAssembler_MeteoblueRs {
		int noRain;		// 0
		int lightRain;	// 1
		int medium;		// 2
		int heavy;		// 3
		int shower;		// 9
	};

	/**
	 * What rain type will be put into anouncement basing on rainspot and
	 * icon
	 */
	enum PlaylistAssembler_RainType {
		RAIN_TYPE_NO_RAIN,	//!<< No rain at all
		RAIN_TYPE_SHOWERS,	//!<< up to half milimeter
		RAIN_TYPE_LIGHT,	//!<< up to 2mm
		RAIN_TYPE_MEDIUM,	//!<< up to 5mm
		RAIN_TYPE_HEAVY_RAIN,		//!<< heavy rainfall
		RAIN_TYPE_HEAVY_SNOWFALL,	//!<< heavy snowfall
		RAIN_TYPE_THUNDERSTORM
	};

	/**
	 * Helper private method for assembling rain forecast
	 * @param forecast
	 * @return
	 */
	PlaylistAssembler_MeteoblueRs putRainForecastFromMeteoblue(std::shared_ptr<org::openapitools::client::model::Inline_response_200> & forecast);



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

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
#include <optional>
#include <vector>

#include "PlaylistSampler.h"
#include "Inline_response_200.h"
#include "ConfigurationFile.h"
#include "StationApi.h"
#include "AprsWXData.h"
#include "AvalancheWarnings.h"
#include "TrendDownloader.h"
#include "EmailDownloaderMessage.h"
#include "SpeechSynthesisResponsivevoice.h"

struct PlaylistAssembler_TextToSpeechAnnouncement_Stats {
	uint32_t added;
	uint32_t addedSingleShot;
	uint32_t tooOld;
	uint32_t singleShotReadBefore;
};

class PlaylistAssembler {

	std::shared_ptr<std::vector<std::string>> playlist;

	std::shared_ptr<PlaylistSampler> & playlistSampler;

	std::shared_ptr<ConfigurationFile> & configurationFile;

	static std::string throwOnEmptyOptional();

	SpeechSynthesisResponsivevoice tts;

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
			std::vector<std::tuple<std::string, AprsWXData>> & weatherlink,
			const std::optional<std::vector<TrendDownloader_Data>> & trend);

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
	 * Takes all downloaded & validated email messages, then converts them into speech
	 * if it is necessary (if a message hasn't been converted yet) and append everything
	 * into playlist
	 * @param messages
	 */
	PlaylistAssembler_TextToSpeechAnnouncement_Stats textToSpeechAnnouncements(std::vector<EmailDownloaderMessage> & messages);

	/**
	 * Appends sign off message
	 */
	void signOff();

	const std::shared_ptr<std::vector<std::string>> getPlaylist() {
		return playlist;
	}
};

#endif /* PLAYLISTASSEMBLER_H_ */

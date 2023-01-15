/*
 * PlaylistAssembler.cpp
 *
 *  Created on: Jan 11, 2023
 *      Author: mateusz
 */

#include "PlaylistAssembler.h"
#include "WrongOrderEx.h"
#include "DefinedStationNotFoundEx.h"

#include <algorithm>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

PlaylistAssembler::PlaylistAssembler(PlaylistSampler & sampler, ConfigurationFile & config) : playlistSampler(sampler), configurationFile(config) {
	// TODO Auto-generated constructor stub

}

PlaylistAssembler::~PlaylistAssembler() {
	// TODO Auto-generated destructor stub
}

PlaylistAssembler::PlaylistAssembler(const PlaylistAssembler &other) : playlistSampler(other.playlistSampler), configurationFile(other.configurationFile) {
	// TODO Auto-generated constructor stub

}

PlaylistAssembler::PlaylistAssembler(PlaylistAssembler &&other) : playlistSampler(other.playlistSampler), configurationFile(other.configurationFile) {
	// TODO Auto-generated constructor stub

}

PlaylistAssembler& PlaylistAssembler::operator=(
		const PlaylistAssembler &other) {
	// TODO Auto-generated method stub

}

PlaylistAssembler& PlaylistAssembler::operator=(PlaylistAssembler &&other) {
	// TODO Auto-generated method stub

}

void PlaylistAssembler::start() {

	SPDLOG_INFO("initializing new playlist.");

	// initialize vector (remove all content if there is any)
	if (playlist.size() > 0) {
		playlist.clear();
	}

	// put an intro
	playlist.push_back(configurationFile.getIntro().ident);

	// put current time if required
	if (configurationFile.getIntro().sayCurrentTime) {
		// get current time anouncement
		std::tuple<std::vector<std::string>, boost::posix_time::ptime> time = playlistSampler.getAudioForCurrentTime();

		// get playlist component, ignore time itself
		std::vector<std::string> time_playlist = std::get<0>(time);

		// move elements from one vector to another
		playlist.insert(playlist.end(), std::make_move_iterator(time_playlist.begin()), std::make_move_iterator(time_playlist.end()));

		SPDLOG_DEBUG("a time has been added: {}", boost::posix_time::to_simple_string(std::get<1>(time)));

	}

}

void PlaylistAssembler::regionalPressure(float pressure) {

	SPDLOG_INFO("assembling announcement for pressure: {}", pressure);

	// generate playlist for rounder pressure
	std::vector<std::string> intermediate = playlistSampler.getAudioListFromNumber((int)std::round(pressure));

	// move that playlist
	playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

	// add "hectopascals"
	playlist.push_back(playlistSampler.getAudioFromUnit(PlaylistSampler_Unit::HPA, (int)pressure));
}

void PlaylistAssembler::currentWeather(
		std::vector<org::openapitools::client::model::Summary> &summary,
		std::vector<AprsWXData> &result) {

	// add current weather anouncement
	playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::CURRENT_WEATHER).value_or(""));

	// iterate through configuration
	for (ConfigurationFile_CurrentWeather w : configurationFile.getCurrent()) {

		SPDLOG_INFO("assembling announcement for source name: {}", w.name);

		// local variables to extract measurements
		float wind_speed = 0.0f, wind_gusts = 0.0f, temperature = 0.0f, pressure = 0.0f;
		int direction = 0, humidity = 0;

		// intermediate vector to store playlist elements
		std::vector<std::string> intermediate;

		// check what type of current weather this is
		if (w.type == ConfigurationFile_CurrentWeatherType::APRX) {

			// find matching weather data from a set parsed from APRX file
			auto weather = std::find_if(result.begin(), result.end(), [& w](AprsWXData & data ) {
				if (data.call == w.name) {		// name in this case will be set to a callsign
					return true;
				}
				else {
					return false;
				}
			});

			// check if data has been found
			if (weather != result.end()) {

				wind_speed = weather->wind_speed;
				wind_gusts = weather->wind_gusts;
				temperature = weather->temperature;
				pressure = weather->pressure;
				direction = weather->wind_direction;
				humidity = weather->humidity;

			}
			else {
				throw DefinedStationNotFoundEx();
			}
		}

		// put anouncement for this station
		playlist.push_back(w.fnIdent);

		// put wind if it is configured
		if (w.sayWind) {
			// say "wind" or "kierunek"
			playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::WIND).value_or(""));

			// say wind direction as name like 'east' or 'north' (not wind direction
			playlist.push_back(playlistSampler.getAudioForWindDirection(direction));

			// say wind speed
			intermediate = playlistSampler.getAudioListFromNumber(wind_speed);
			playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

		}
	}

}

void PlaylistAssembler::forecastMeteoblue(
		std::vector<org::openapitools::client::model::Inline_response_200> forecasts) {
}

void PlaylistAssembler::recordedAnnouncement(bool preOrPost) {
}

void PlaylistAssembler::signOff() {
}

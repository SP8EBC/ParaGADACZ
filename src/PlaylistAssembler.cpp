/*
 * PlaylistAssembler.cpp
 *
 *  Created on: Jan 11, 2023
 *      Author: mateusz
 */

#include "PlaylistAssembler.h"
#include "ForecastFinder.h"

#include "WrongOrderEx.h"
#include "DefinedStationNotFoundEx.h"
#include "AudioFileNotFoundEx.h"
#include "UnknownStationTypeEx.h"

#include <algorithm>

#include <boost/algorithm/string.hpp>

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

	playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::REGIONAL_QNH).value_or(throwOnEmptyOptional()));

	// generate playlist for rounder pressure
	std::vector<std::string> intermediate = playlistSampler.getAudioListFromNumber((int)std::round(pressure));

	// move that playlist
	playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

	// add "hectopascals"
	playlist.push_back(playlistSampler.getAudioFromUnit(PlaylistSampler_Unit::HPA, (int)pressure));
}

void PlaylistAssembler::currentWeather(
		std::vector<std::pair<std::string, org::openapitools::client::model::Summary>> & summary,
		std::vector<AprsWXData> & result) {

	// add current weather anouncement
	playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::CURRENT_WEATHER).value_or(throwOnEmptyOptional()));

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
				if (boost::to_upper_copy(data.call) == boost::to_upper_copy(w.name)) {		// name in this case will be set to a callsign
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
				SPDLOG_ERROR("DefinedStationNotFoundEx, w.type: {}, w.name: {}", w.type, w.name);

				throw DefinedStationNotFoundEx();
			}
		}

		else if (w.type == ConfigurationFile_CurrentWeatherType::POGODA_CC) {
			// find matching weather data from a set parsed from APRX file
			auto weather = std::find_if(
					summary.begin(),
					summary.end(),
					[& w](std::pair<std::string, org::openapitools::client::model::Summary> summary_pair)
					{
						if (boost::to_upper_copy(summary_pair.first) == boost::to_upper_copy(w.name)) {		// name in this case will be set to a callsign
							return true;
						}
						else {
							return false;
						}
					});

			// check if data has been found
			if (weather != summary.end()) {

				wind_speed = weather->second.getAverageSpeed();
				wind_gusts = weather->second.getGusts();
				temperature = weather->second.getAvgTemperature();
				pressure = weather->second.getQnh();
				direction = weather->second.getDirection();
				humidity = weather->second.getHumidity();

			}
			else {
				SPDLOG_ERROR("DefinedStationNotFoundEx, w.type: {}, w.name: {}", w.type, w.name);

				throw DefinedStationNotFoundEx();
			}
		}
		else {
			SPDLOG_ERROR("UnknownStationTypeEx, w.type: {}", w.type);

			throw UnknownStationTypeEx();
		}

		// put anouncement for this station
		playlist.push_back(w.fnIdent);

		// put wind if it is configured
		if (w.sayWind) {
			SPDLOG_INFO("appending wind speed {} for station: {}", wind_speed, w.name);

			// say "wind" or "kierunek"
			playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::WIND).value_or(throwOnEmptyOptional()));

			// say wind direction as name like 'east' or 'north' (not degrees)
			playlist.push_back(playlistSampler.getAudioForWindDirection(direction));

			// say wind speed in m/s
			intermediate = playlistSampler.getAudioListFromNumber(wind_speed);
			playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist.push_back(playlistSampler.getAudioFromUnit(PlaylistSampler_Unit::MS, (int)wind_speed));

			// say "gusts" or "porywy"
			playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::WIND_GUSTS).value_or(throwOnEmptyOptional()));

			// and gust value
			intermediate = playlistSampler.getAudioListFromNumber(wind_gusts);
			playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// unit
			playlist.push_back(playlistSampler.getAudioFromUnit(PlaylistSampler_Unit::MS, (int)wind_gusts));

		}

		if (w.sayTemperature) {
			SPDLOG_INFO("appending temperature {} for station: {}", temperature,  w.name);

			// say "temperature
			playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::TEMPERATURE).value_or(throwOnEmptyOptional()));

			// say wind temperature but round to integer
			intermediate = playlistSampler.getAudioListFromNumber((int)std::round(temperature));
			playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist.push_back(playlistSampler.getAudioFromUnit(PlaylistSampler_Unit::CELSIUS, (int)wind_speed));

		}

		if (w.sayHumidy) {
			SPDLOG_INFO("appending humidity {} for station: {}", humidity, w.name);

			// say "temperature
			playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::HUMIDITY).value_or(throwOnEmptyOptional()));

			// say wind temperature but round to integer
			intermediate = playlistSampler.getAudioListFromNumber(humidity);
			playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist.push_back(playlistSampler.getAudioFromUnit(PlaylistSampler_Unit::PERCENT, (int)wind_speed));
		}

		if (w.sayPressure) {
			SPDLOG_INFO("appending pressure {} for station: {}", pressure, w.name);

			// say "pressure
			playlist.push_back(playlistSampler.getConstantElement(PlaylistSampler_ConstanElement::HUMIDITY).value_or(throwOnEmptyOptional()));

			// say wind temperature but round to integer
			intermediate = playlistSampler.getAudioListFromNumber((int)std::round(pressure));
			playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist.push_back(playlistSampler.getAudioFromUnit(PlaylistSampler_Unit::CELSIUS, (int)wind_speed));
		}
	}

}

void PlaylistAssembler::forecastMeteoblue(
		std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> forecasts) {

	// generate forecast anouncement
	std::optional<std::vector<std::string>> intermediate = playlistSampler.getAudioForForecastAnouncement(configurationFile.getForecast().futureTime);

	// and put this into the playlist
	if (intermediate.has_value()) {
		playlist.insert(playlist.end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));
	}
	else {
		throw new AudioFileNotFoundEx();
	}

	// iterate through all configured locations for forecast
	for (ConfigurationFile_ForecastMeteoblue_Locations location : configurationFile.getForecast().locations) {

		// location from the configuration, a name of forecast to find (in uppercase)
		std::string nameUppercase = boost::to_upper_copy(location.name);

		// look for matching forecast in API response
		auto found = std::find_if(forecasts.begin(), forecasts.end(), [& location](std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>> r) {
			std::string locationUppercase = boost::to_upper_copy(location.name);

			std::string forecastUppercase = boost::to_upper_copy(r.first);

			// check if this is what we're looking for
			if (forecastUppercase == locationUppercase) {
				return true;
			}
			else {
				return false;
			}

		});

		// check if anything has been found
		if (found == forecasts.end()) {
			SPDLOG_ERROR("DefinedStationNotFoundEx, location.name: {}", location.name);

			throw DefinedStationNotFoundEx();
		}

		// extract forecast API response from the pair
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> foundForecast = found->second;


		if (location.sayWind) {

		}

		if (location.sayTemperature) {
			std::tuple<int64_t, float> temperature = ForecastFinder::getTemperatureMeteoblue(foundForecast, configurationFile.getForecast().futureTime);

		}
	}
}

void PlaylistAssembler::recordedAnnouncement(bool preOrPost) {

	std::vector<std::string> filesToAppend;

	if (preOrPost) {
		filesToAppend = configurationFile.getRecordedSpecialAnnouncementPost();
	}
	else {
		filesToAppend = configurationFile.getRecordedSpecialAnnouncementPre();
	}

	for (std::string file : filesToAppend) {
		playlist.push_back(file);
	}

}

std::string PlaylistAssembler::throwOnEmptyOptional() {
	throw AudioFileNotFoundEx();
}

void PlaylistAssembler::signOff() {
}

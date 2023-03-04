/*
 * PlaylistAssembler.cpp
 *
 *	This class takes all numeric data and then assemble complete playlist using given playlist sampler
 *
 *
 *  Created on: Jan 11, 2023
 *      Author: mateusz
 */

#include "PlaylistAssembler.h"
#include "ForecastFinder.h"
#include "main.h"

#include "exception/WrongOrderEx.h"
#include "exception/DefinedStationNotFoundEx.h"
#include "exception/AudioFileNotFoundEx.h"
#include "exception/UnknownStationTypeEx.h"

#include <algorithm>
#include <functional>

#include <boost/algorithm/string.hpp>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

struct Checker
{
    template<typename T>
    operator T() const
    {
    	SPDLOG_ERROR("AudioFileNotFoundEx!!");
    	throw AudioFileNotFoundEx();
    }
};
constexpr Checker checker{};

PlaylistAssembler::PlaylistAssembler(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config) : playlistSampler(sampler), configurationFile(config) {

	this->playlist = std::make_shared<std::vector<std::string>>();
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

void PlaylistAssembler::start() {

	SPDLOG_INFO("initializing new playlist->");

	// initialize vector (remove all content if there is any)
	if (playlist->size() > 0) {
		playlist->clear();
	}

	//put an intro
	auto intro = configurationFile->getIntro().ident;
	playlist->insert(playlist->end(), std::make_move_iterator(intro.begin()), std::make_move_iterator(intro.end()));

	// put current time if required
	if (configurationFile->getIntro().sayCurrentTime) {
		// get current time anouncement
		std::tuple<std::vector<std::string>, boost::posix_time::ptime> time = playlistSampler->getAudioForCurrentTime();

		// get playlist component, ignore time itself
		std::vector<std::string> time_playlist = std::get<0>(time);

		// move elements from one vector to another
		playlist->insert(playlist->end(), std::make_move_iterator(time_playlist.begin()), std::make_move_iterator(time_playlist.end()));

		SPDLOG_DEBUG("a time has been added: {}", boost::posix_time::to_simple_string(std::get<1>(time)));

	}

}

void PlaylistAssembler::regionalPressure(float pressure) {

	SPDLOG_INFO("assembling announcement for pressure: {}", pressure);

	playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::REGIONAL_QNH).value_or(checker));

	// generate playlist for rounder pressure
	std::vector<std::string> intermediate = playlistSampler->getAudioListFromNumber((int)std::round(pressure));

	// move that playlist
	playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

	// add "hectopascals"
	playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::HPA, (int)pressure));
}

void PlaylistAssembler::currentWeather(
		std::vector<std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>>> & summary,
		std::vector<AprsWXData> & result) {

	// add current weather anouncement
	playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::CURRENT_WEATHER).value_or(checker));

	// iterate through configuration
	for (ConfigurationFile_CurrentWeather w : configurationFile->getCurrent()) {

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
#ifdef MAIN_FAIL_ON_MISSING_CURRENT_CONDITIONS
				throw DefinedStationNotFoundEx();
#else
				continue;
#endif
			}
		}

		else if (w.type == ConfigurationFile_CurrentWeatherType::POGODA_CC) {
			// find matching weather data from a set parsed from APRX file
			auto weather = std::find_if(
					summary.begin(),
					summary.end(),
					[& w](std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>> summary_pair)
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

				wind_speed = weather->second->getAverageSpeed();
				wind_gusts = weather->second->getGusts();
				temperature = weather->second->getAvgTemperature();
				pressure = weather->second->getQnh();
				direction = weather->second->getDirection();
				humidity = weather->second->getHumidity();

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
		playlist->push_back(w.fnIdent);

		// put wind if it is configured
		if (w.sayWind) {
			SPDLOG_INFO("appending wind speed {} for station: {}", wind_speed, w.name);

			// say "wind" or "kierunek"
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::WIND).value_or(checker));

			// say wind direction as name like 'east' or 'north' (not degrees)
			playlist->push_back(playlistSampler->getAudioForWindDirection(direction));

			// say wind speed in m/s
			intermediate = playlistSampler->getAudioListFromNumber(wind_speed);
			playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MS, (int)wind_speed));

			// say "gusts" or "porywy"
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::WIND_GUSTS).value_or(checker));

			// and gust value
			intermediate = playlistSampler->getAudioListFromNumber(wind_gusts);
			playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// unit
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MS, (int)wind_gusts));

		}

		if (w.sayTemperature) {
			SPDLOG_INFO("appending temperature {} for station: {}", temperature,  w.name);

			// say "temperature
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::TEMPERATURE).value_or(checker));

			// say wind temperature but round to integer
			intermediate = playlistSampler->getAudioListFromNumber((int)std::round(temperature));
			playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::DEG, (int)temperature));
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::CELSIUS, (int)temperature));

		}

		if (w.sayHumidy) {
			SPDLOG_INFO("appending humidity {} for station: {}", humidity, w.name);

			// say "temperature
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::HUMIDITY).value_or(checker));

			// say wind temperature but round to integer
			intermediate = playlistSampler->getAudioListFromNumber(humidity);
			playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::PERCENT, (int)wind_speed));
		}

		if (w.sayPressure) {
			SPDLOG_INFO("appending pressure {} for station: {}", pressure, w.name);

			// say "pressure
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::HUMIDITY).value_or(checker));

			// say pressure, but round to integer
			intermediate = playlistSampler->getAudioListFromNumber((int)std::round(pressure));
			playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

			// and the unit itself
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::HPA, (int)pressure));
		}
	}

}

void PlaylistAssembler::forecastMeteoblue(
		std::vector<std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>>> & forecasts) {

	// generate forecast anouncement
	std::optional<std::vector<std::string>> intermediate = playlistSampler->getAudioForForecastAnouncement(configurationFile->getForecast().futureTime);

	// and put this into the playlist
	if (intermediate.has_value()) {
		playlist->insert(playlist->end(), std::make_move_iterator(intermediate->begin()), std::make_move_iterator(intermediate->end()));
	}
	else {
		throw new AudioFileNotFoundEx();
	}

	// iterate through all configured locations for forecast
	for (ConfigurationFile_ForecastMeteoblue_Locations location : configurationFile->getForecast().locations) {

		// location from the configuration, a name of forecast to find (in uppercase)
		std::string nameUppercase = boost::to_upper_copy(location.name);

		SPDLOG_INFO("assembling announcement for Meteoblue weather forecast, location.name: {}", location.name);

		// look for matching forecast in API response
		auto found = std::find_if(forecasts.begin(), forecasts.end(), [& location](std::tuple<std::string, std::shared_ptr<org::openapitools::client::model::Inline_response_200>> r) {
			std::string locationUppercase = boost::to_upper_copy(location.name);

			std::string forecastUppercase = boost::to_upper_copy(std::get<std::string>(r));

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
#ifdef MAIN_FAIL_ON_MISSING_FORECAST
			throw DefinedStationNotFoundEx();
#else
			continue;
#endif
		}

		// place location announcement for this forecast
		std::optional<std::string> forecastPointAudioFile = playlistSampler->getAudioForForecastName(location.name);

		if (!forecastPointAudioFile.has_value()) {
			SPDLOG_ERROR("AudioFileNotFoundEx, location.name: {}", location.name);

			throw AudioFileNotFoundEx();
		}

		// append location anouncement into playlist (if it is found)
		playlist->push_back(*forecastPointAudioFile);

		// extract forecast API response from the pair
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> foundForecast = std::get<1>(*found);


		if (location.sayWind) {
			std::tuple<int64_t, float> wind = ForecastFinder::getWindSpeedMeteoblue(foundForecast, configurationFile->getForecast().futureTime);
			std::tuple<int64_t, float> windDirection = ForecastFinder::getWindDirectionMeteoblue(foundForecast, configurationFile->getForecast().futureTime);

			SPDLOG_INFO("appending Meteoblue wind forecast, wind: {}, windDirection: {}", std::get<1>(wind), std::get<1>(windDirection));

			// "wind"
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::WIND).value());

			// convert wind direction to audio file
			std::string windDirectionAudioFile = playlistSampler->getAudioForWindDirection(std::get<1>(windDirection));

			// and append that to main playlist
			playlist->push_back(windDirectionAudioFile);

			// convert wind speed to playlist
			auto windAudioFile = playlistSampler->getAudioListFromNumber(std::get<1>(wind));

			// and append that to main playlist
			playlist->insert(playlist->end(), std::make_move_iterator(windAudioFile.begin()), std::make_move_iterator(windAudioFile.end()));

			// and the unit itself
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MS, (int)std::get<1>(wind)));

		}

		if (location.sayTemperature) {
			std::tuple<int64_t, float> temperature = ForecastFinder::getTemperatureMeteoblue(foundForecast, configurationFile->getForecast().futureTime);

			// "temperature"
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::TEMPERATURE).value());

			auto _temperature = std::get<1>(temperature);

			SPDLOG_INFO("appending wind forecast, temperature: {}", _temperature);
			// convert temperature to playlist
			auto temperatureAudioFile = playlistSampler->getAudioListFromNumber(_temperature);

			// and append that to main playlist
			playlist->insert(playlist->end(), std::make_move_iterator(temperatureAudioFile.begin()), std::make_move_iterator(temperatureAudioFile.end()));

			// and the unit itself
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::DEG, (int)_temperature));
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::CELSIUS, (int)_temperature));

		}
	}
}

void PlaylistAssembler::recordedAnnouncement(bool preOrPost) {

	std::vector<std::string> filesToAppend;

	if (preOrPost) {
		filesToAppend = configurationFile->getRecordedSpecialAnnouncementPost();
	}
	else {
		filesToAppend = configurationFile->getRecordedSpecialAnnouncementPre();
	}

	for (std::string file : filesToAppend) {
		playlist->push_back(file);
	}

}

void PlaylistAssembler::avalancheWarning(AvalancheWarnings_Location location,
		int8_t level, AvalancheWarnings_Expositions expositions)
{
	// level 5 is non existing in Poland, level 0 is either no
	// avalanche warning at all or there is no snow
	if (level > 0 && level < 5) {

		auto audio_for_location = playlistSampler->getAudioForAvalancheWarningLocation(location);

		// check if this location exists
		if (audio_for_location.has_value()) {
			// avalanche warnings announcement
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::AVALANCHE_WARNING).value());

			// location for this anouncement
			playlist->push_back(*audio_for_location);

			// severity level
			switch (level) {
			case 1:	playlist->push_back(*(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::FIRST_LEVEL))); break;
			case 2: playlist->push_back(*(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::SECOND_LEVEL))); break;
			case 3: playlist->push_back(*(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::THIRD_LEVEL))); break;
			case 4: playlist->push_back(*(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::FOURTH_LEVEL))); break;
			default: break;
			}

			// check if there is any dangerous exposition
			if (expositions.hasExposition()) {

				volatile int howMany = expositions.howMany();

				// exposition anouncement
				playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::DANGEROUS_EXPOSITION).value());

				// north exposition
				if (expositions.north) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(1));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}

				if (expositions.northEast) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(46));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}

				if (expositions.east) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(91));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}

				if (expositions.southEast) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(136));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}

				if (expositions.south) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(181));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}

				if (expositions.southWest) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(226));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}

				if (expositions.west) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(271));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}

				if (expositions.northWest) {
					playlist->push_back(playlistSampler->getAudioForWindDirection(316));

					if ((--howMany) > 0) {
						playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ALSO).value());
					}
				}
			}
		}
		else {
			SPDLOG_ERROR("No audio file for this avalanche location was found");
		}
	}
	else {
		SPDLOG_WARN("There is no warining for this location due to wrong or non existing severity level");
	}
}

void PlaylistAssembler::signOff() {
}

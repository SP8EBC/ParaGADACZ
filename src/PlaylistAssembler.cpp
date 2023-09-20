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
#include "TimeTools.h"
#include "MeteoblueRainParser.h"
#include "main.h"

#include "exception/WrongOrderEx.h"
#include "exception/DefinedStationNotFoundEx.h"
#include "exception/AudioFileNotFoundEx.h"
#include "exception/UnknownStationTypeEx.h"

#include <algorithm>
#include <functional>

#include <boost/algorithm/string.hpp>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

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

/**
 * Used to extract string representing station name from different kinds of results got from different sources
 */
struct PlaylistAssembler_WxStationNameToString {

	// POGODA_CC
	static std::string toString(const std::pair<std::string, std::shared_ptr<org::openapitools::client::model::Summary>> & summary_pair) {
		return summary_pair.first;
	}

	// APRX
	static std::string toString(const AprsWXData & data) {
		return data.call;
	}

	// WEATHERLINK
	static std::string toString(const std::tuple<std::string, AprsWXData> & weatherlink) {
		return std::get<0>(weatherlink);
	}
};

/**
 * Unary predicate to look for given station using std::find_if algorithm
 */
struct PlaylistAssembler_UnaryPredicate {

	std::string whatToLookFor;

	PlaylistAssembler_UnaryPredicate(std::string _whatToLookFor) : whatToLookFor(boost::to_upper_copy(_whatToLookFor)) {};

	template<class T>
	bool operator()(const T t) {
		bool out = false;

		if (boost::to_upper_copy(PlaylistAssembler_WxStationNameToString::toString(t)) == whatToLookFor) {		// name in this case will be set to a callsign
			out = true;
		}
		else {
			out = false;
		}

		return out;
	}
};

/**
 * Used to look for matching trend data
 */
struct PlaylistAssembler_TrendUnaryPredicate {
	std::string whatToLookFor;

	PlaylistAssembler_TrendUnaryPredicate(std::string _whatToLookFor) : whatToLookFor(_whatToLookFor) {};

	bool operator()(const TrendDownloader_Data & trend) {
		if (trend.stationName == whatToLookFor) {
			return true;
		}
		else {
			return false;
		}
	}
};

PlaylistAssembler::PlaylistAssembler(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config) : playlistSampler(sampler), configurationFile(config), tts("kvfbSITh") {

	this->playlist = std::make_shared<std::vector<std::string>>();
}

PlaylistAssembler::~PlaylistAssembler() {
	// TODO Auto-generated destructor stub
}

PlaylistAssembler::PlaylistAssembler(const PlaylistAssembler &other) : playlistSampler(other.playlistSampler), configurationFile(other.configurationFile), tts("kvfbSITh") {
	// TODO Auto-generated constructor stub

}

PlaylistAssembler::PlaylistAssembler(PlaylistAssembler &&other) : playlistSampler(other.playlistSampler), configurationFile(other.configurationFile), tts("kvfbSITh") {
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
		std::vector<AprsWXData> & result,
		std::vector<std::tuple<std::string, AprsWXData>> & weatherlink,
		const std::optional<std::vector<TrendDownloader_Data>> & trend) {

	bool includeTrend = false;

	// add current weather anouncement
	playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::CURRENT_WEATHER).value_or(checker));

	// check if there is trend put as an input
	if (trend.has_value()) {
		includeTrend = true;
	}

	// iterate through configuration
	for (ConfigurationFile_CurrentWeather w : configurationFile->getCurrent()) {

		PlaylistAssembler_UnaryPredicate pred(w.name);
		PlaylistAssembler_TrendUnaryPredicate trendPred(w.name);

		SPDLOG_INFO("assembling announcement for source name: {}", w.name);

		// local variables to extract measurements
		float wind_speed = 0.0f, wind_gusts = 0.0f, temperature = 0.0f, pressure = 0.0f;
		int direction = 0, humidity = 0;

		// a timestamp marking how many seconds ago this frame has been received from radio
		long timestamp = 0;

		// intermediate vector to store playlist elements
		std::vector<std::string> intermediate;

		// check what type of current weather this is
		if (w.type == ConfigurationFile_CurrentWeatherType::APRX) {

			auto weather = std::find_if(result.begin(), result.end(), pred);

			// check if data has been found
			if (weather != result.end()) {

				// get timestamp depends on how a clock on APRX host is configured
				if (this->configurationFile->getAprxConfig().aprxRfLogTimeInLocal) {
					timestamp = weather->packetAgeInSecondsLocal;
				}
				else {
					timestamp = weather->packetAgeInSecondsUtc;
				}

				// check if this data is not too old to be used
				if (timestamp < this->configurationFile->getMaximumDataAgeInMinutes() * 60) {
					wind_speed = weather->wind_speed;
					wind_gusts = weather->wind_gusts;
					temperature = weather->temperature;
					pressure = weather->pressure;
					direction = weather->wind_direction;
					humidity = weather->humidity;
				}
				else {
					SPDLOG_WARN("Data for station {} from aprx rf-log are too old and cannot be used", w.name);
					continue;
				}


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

		else if (w.type == ConfigurationFile_CurrentWeatherType::DAVIS) {

			auto weather = std::find_if(weatherlink.begin(), weatherlink.end(), pred);

			if (weather != weatherlink.end()) {
				// get data
				const AprsWXData data = std::get<1>(*weather);

				// get data age in seconds, do not need to care about timezones
				timestamp = data.packetAgeInSecondsLocal;

				// check if data are not too old
				if (timestamp < this->configurationFile->getMaximumDataAgeInMinutes() * 60) {
					wind_speed = data.wind_speed;
					wind_gusts = data.wind_gusts;
					temperature = data.temperature;
					pressure = data.pressure;
					direction = data.wind_direction;
					humidity = data.humidity;
				}
				else {
					SPDLOG_WARN("Data for Davis weatherlink station {} are too old and cannot be used", w.name);

					// go to next station
					continue;
				}

			}
		}

		else if (w.type == ConfigurationFile_CurrentWeatherType::POGODA_CC) {
			// find matching weather data from a set parsed from APRX file
			auto weather = std::find_if(summary.begin(), summary.end(), pred);


			// check if data has been found
			if (weather != summary.end()) {

				wind_speed = weather->second->getAverageSpeed();
				wind_gusts = weather->second->getGusts();
				temperature = weather->second->getAvgTemperature();
				pressure = weather->second->getQnh();
				direction = weather->second->getDirection();
				humidity = weather->second->getHumidity();

				// get timestamp of last measurement
				timestamp = weather->second->getLastTimestamp();

				if (TimeTools::getEpoch() - timestamp > this->configurationFile->getMaximumDataAgeInMinutes() * 60) {
					SPDLOG_ERROR("Weather data from pogoda.cc station {} is too old and cannot be used.", w.name);

					continue;
				}

				// check quality factors

				if (!configurationFile->getPogodaCc().ignoreTemperatureQf) {
					// temperature
					if (weather->second->getTemperatureQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_FULL_ ||
						weather->second->getTemperatureQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_DEGRADED_)
					{
						//w.sayTemperature = true;
					}
					else
					{
						w.sayTemperature = false;
						SPDLOG_WARN("Temperature for station {} won't be said due to quality factor", w.name);
					}
				}

				if (!configurationFile->getPogodaCc().ignoreWindQf) {
					// wind
					if (weather->second->getWindQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_FULL_ ||
						weather->second->getWindQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_DEGRADED_)
					{
						//w.sayTemperature = true;
					}
					else
					{
						w.sayTemperature = false;
						SPDLOG_WARN("Temperature for station {} won't be said due to quality factor", w.name);
					}
				}

				if (!configurationFile->getPogodaCc().ignorePressureQf) {
					// pressure
					if (weather->second->getQnhQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_FULL_ ||
						weather->second->getQnhQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_DEGRADED_)
					{
						//w.sayPressure = true;
					}
					else
					{
						w.sayPressure = false;
						SPDLOG_WARN("Pressure for station {} won't be said due to quality factor", w.name);
					}
				}

				if (!configurationFile->getPogodaCc().ignoreHumidityQf) {
					// humidity
					if (weather->second->getHumidityQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_FULL_ ||
						weather->second->getHumidityQf()->getValue() == org::openapitools::client::model::QualityFactor::eQualityFactor::QualityFactor_DEGRADED_)
					{
						//w.sayHumidy = true;
					}
					else
					{
						w.sayHumidy = false;
						SPDLOG_WARN("Humidity for station {} won't be said due to quality factor", w.name);
					}
				}

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
			SPDLOG_INFO("appending wind speed {} and wind gusts {} for station: {}", wind_speed, wind_gusts, w.name);

			// say "wind" or "kierunek"
			playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::WIND).value_or(checker));

			if (w.doesntSayWinddir) {
				SPDLOG_DEBUG("Wind direction anouncement will not be appended to playlist!");
			}
			else {
				// say wind direction as name like 'east' or 'north' (not degrees)
				playlist->push_back(playlistSampler->getAudioForWindDirection(direction));
			}

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

			// check if trend is enabled
			if (w.sayTrend && includeTrend && configurationFile->getTrend().enabledWindspeed) {
				// look for trend for this station
				auto trendForStation = std::find_if(trend->begin(), trend->end(), trendPred);

				if (trendForStation != trend->end()) {
					// calculate a difference
					float diff = wind_speed - trendForStation->windspeedTrend;

					// check if change of windspeed is big enought to include an anouncement
					if (::abs(diff) > ::abs(::round(wind_speed)) * (configurationFile->getTrend().minimumWindChange / 100.0f)) {
						SPDLOG_INFO("appending wind speed trend {} for station: {}", diff, w.name);

						intermediate = playlistSampler->getAudioForTrendAnouncement(configurationFile->getTrend().trendLenghtInHours, diff, PlaylistSampler_Unit::MS);

					}
					else {
						SPDLOG_INFO("Wind difference of {} is too low to include trend anouncement", diff);

						intermediate = playlistSampler->getAudioForTrendAnouncement(configurationFile->getTrend().trendLenghtInHours, 0.0f, PlaylistSampler_Unit::MS);
					}

					// append samples to the main playlist
					playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));
				}
				else {
					SPDLOG_ERROR("No trend data has been found for station {}", w.name);
				}

			}
			else {
				SPDLOG_DEBUG("Windspeed trend is not configured, w.sayTrend: {}, includeTrend: {}, enabledTemperature: {}", w.sayTrend, includeTrend, configurationFile->getTrend().enabledWindspeed);
			}

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

			// check if trend is enabled
			if (w.sayTrend && includeTrend && configurationFile->getTrend().enabledTemperature) {
				// look for trend for this station
				auto trendForStation = std::find_if(trend->begin(), trend->end(), trendPred);

				if (trendForStation != trend->end()) {
					// calculate a difference
					float diff = temperature - trendForStation->temperatureTrend;

					// check if change of windspeed is big enought to include an anouncement
					if (::abs(diff) > ::abs(::round(temperature)) * (configurationFile->getTrend().minimumTemperatureChange / 100.0f)) {
						SPDLOG_INFO("appending temperature trend {} for station: {}", diff, w.name);

						intermediate = playlistSampler->getAudioForTrendAnouncement(configurationFile->getTrend().trendLenghtInHours, diff, PlaylistSampler_Unit::CELSIUS);

					}
					else {
						SPDLOG_INFO("Temperature of {} is too low to include trend anouncement", diff);

						intermediate = playlistSampler->getAudioForTrendAnouncement(configurationFile->getTrend().trendLenghtInHours, 0.0f, PlaylistSampler_Unit::CELSIUS);
					}

					// append samples to the main playlist
					playlist->insert(playlist->end(), std::make_move_iterator(intermediate.begin()), std::make_move_iterator(intermediate.end()));

				}
				else {
					SPDLOG_ERROR("No trend data has been found for station {}", w.name);
				}

			}
			else {
				SPDLOG_DEBUG("Temeprature trend is not configured, w.sayTrend: {}, includeTrend: {}, enabledTemperature: {}", w.sayTrend, includeTrend, configurationFile->getTrend().enabledTemperature);
			}

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


	std::vector<std::string> listOne = playlistSampler->getAudioListFromNumber(1);
	std::vector<std::string> listThree = playlistSampler->getAudioListFromNumber(3);
	std::vector<std::string> listFive = playlistSampler->getAudioListFromNumber(5);

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

			SPDLOG_INFO("appending Meteoblue wind forecast, wind speed: {}, windDirection: {}", std::get<1>(wind), std::get<1>(windDirection));

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

			float _temperature = ::round(std::get<1>(temperature));

			SPDLOG_INFO("appending Meteoblue temperature forecast, temperature: {}", (int)_temperature);
			// convert temperature to playlist
			auto temperatureAudioFile = playlistSampler->getAudioListFromNumber((int)_temperature);

			// and append that to main playlist
			playlist->insert(playlist->end(), std::make_move_iterator(temperatureAudioFile.begin()), std::make_move_iterator(temperatureAudioFile.end()));

			// and the unit itself
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::DEG, (int)_temperature));
			playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::CELSIUS, (int)_temperature));

		}

		if (location.sayPrecipation) {
			MeteoblueRainParser::MeteoblueRainParser_PrecipType precipation = MeteoblueRainParser::getRainForecastFromMeteoblue(foundForecast, configurationFile);

			SPDLOG_INFO("appending Meteoblue precipation forecast, precipation: {}", (int)precipation);

			switch (precipation) {
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_NO_PRECIPATION:	//!<< No rain or snow at all
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::NO_PRECIPATION).value());
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_LOCAL_INTERMITTEND:	//!<< Some precipations in vicinity
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::INTERMITTENT).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::LOCAL).value());
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_SHOWERS_RAIN:		//!<< rain up to one milimeter
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listOne.begin(),  listOne.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)1));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_SHOWERS_SNOW:		//!<< snow up to one milimeter
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::SNOW).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listOne.begin(),  listOne.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)1));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_LIGHT_RAIN:			//!<< rain up to 3mm
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listThree.begin(),  listThree.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)3));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_LIGHT_SNOW:			//!<< snow up to 3mm
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::SNOW).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listThree.begin(),  listThree.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)3));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_MEDIUM_RAIN:		//!<< rain up to 5mm
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listFive.begin(),  listFive.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)5));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_MEDIUM_SNOW:		//!<< snow up to 5mm
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::SNOW).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listFive.begin(),  listFive.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)5));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_HEAVY_RAIN:			//!<< heavy rainfall
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ABOVE).value());
					playlist->insert(playlist->end(), listFive.begin(),  listFive.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)5));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_HEAVY_SNOWFALL:		//!<< heavy snowfall
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::SNOW).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ABOVE).value());
					playlist->insert(playlist->end(), listFive.begin(),  listFive.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)5));
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_LIGHT_RAIN_THUNDER: //!<< rain up to 3mm + thunderstorm possible
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listThree.begin(),  listThree.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)3));
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::POSSIBLE).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::THUNDERSTORM).value());
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_MEDIUM_RAIN_THUNDER: //!<< rain up to 5mm + thunderstorm possible
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::UO_TO).value());
					playlist->insert(playlist->end(), listFive.begin(),  listFive.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)5));
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::POSSIBLE).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::THUNDERSTORM).value());
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_HEAVY_RAIN_THUNDER: //!<< rain up to 3mm + thunderstorm possible
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::ABOVE).value());
					playlist->insert(playlist->end(), listFive.begin(),  listFive.end());
					playlist->push_back(playlistSampler->getAudioFromUnit(PlaylistSampler_Unit::MILIMETER, (int)5));
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::POSSIBLE).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::THUNDERSTORM).value());
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_LOCAL_THUNDERSTORM:	//!<< thunderstorm possible
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::INTERMITTENT).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::PRECIPATION).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::RAIN).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::POSSIBLE).value());
					playlist->push_back(playlistSampler->getConstantElement(PlaylistSampler_ConstanElement::THUNDERSTORM).value());
					break;
				case MeteoblueRainParser::MeteoblueRainParser_PrecipType::RAIN_TYPE_UNKNOWN:		//!<< thunderstorm possible
					SPDLOG_WARN("Weird rain forecast data!");
					break;
				default:
					SPDLOG_ERROR("Unknown and unusable rain forecast data!");
			}


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

void PlaylistAssembler::textToSpeechAnnouncements(
		std::vector<EmailDownloaderMessage> &messages) {

	const std::string indexFilename = "";
	const uint32_t ignoreOlderThan = 240;
	const SpeechSynthesis_Language lang = SPEECH_ENGLISH;

	const uint64_t currentTime = TimeTools::getEpoch();

	try {
		// try to read an index
		const int readResult = tts.readIndex(indexFilename);

		// index hasn't een created yet
		if (readResult == -1) {
			// create new file, it will be opened automatically
			tts.createIndex(indexFilename);
		}
	}
	catch (std::runtime_error & ex) {
		// something went wrong while loading text-to-speech results
		// so reainitialize cache index
		tts.createIndex(indexFilename);
	}
	catch (std::invalid_argument & ex) {
		// path used as an index file is incorrect. maybe it points to a directory
		// or devide, not to regular file
		SPDLOG_ERROR("Text-to-speech conversion cannot continue");
		throw ex; // bail out
	}

	// convert all emails into speech
	tts.convertEmailsToSpeech(messages, ignoreOlderThan, lang);

	// get an index after conversion, all messages should be here
	const std::list<SpeechSynthesis_MessageIndexElem>& index = tts.getIndexContent();

	// go through emails
	for (SpeechSynthesis_MessageIndexElem elem : index) {
		// check if this is not single shot message
		if (elem.sayUntil != EMAILDOWNLOADERMESSAGE_VALIDUNTIL_SINGLESHOT_ANNOUNCEMENT) {
			// check if this announcement can still be played
			if (elem.sayUntil < currentTime) {
				continue;	// this is too old, so skip it
			}
		}
		else {
			// if this is a single shot message check if it has been read before
			if (elem.receivedAt == 0) {
				continue;
			}

		}

		// append this file
		playlist->push_back(elem.filename);
	}


}

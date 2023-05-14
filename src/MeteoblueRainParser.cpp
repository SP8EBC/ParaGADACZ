/*
 * MeteoblueRainParser.cpp
 *
 *  Created on: May 8, 2023
 *      Author: mateusz
 */

#include "MeteoblueRainParser.h"
#include "ForecastFinder.h"

MeteoblueRainParser::MeteoblueRainParser() {
	// TODO Auto-generated constructor stub

}

MeteoblueRainParser::MeteoblueRainParser_PrecipType MeteoblueRainParser::parse(
		MeteobluePictocode pictocode,
		std::string rainspot
		) {

	MeteoblueRainParser_PrecipType out;

	MeteoblueRainParser_Rs rainspotStats;
	memset(&rainspotStats, 0x00, sizeof(MeteoblueRainParser_Rs));

	// go through rainspot string and count each rain forecast point
	for (char s : rainspot) {

		if (s == '0') {
			rainspotStats.noRain++;
		}
		else if (s == '1') {
			rainspotStats.lightRain++;
		}
		else if (s == '2') {
			rainspotStats.medium++;
		}
		else if (s == '3') {
			rainspotStats.heavy++;
		}
		else if (s == '9') {
			rainspotStats.shower++;
		}
		/**
		 * 0 = no rain, 1 = light rain (0.2 – 1.5 mm), 2 = medium (1.5 – 5 mm), 3 = heavy (>5 mm), 9 = shower (0.02 – 0.2 mm)
		 */
	}

	// sum all rainspots with appropriate weight
	const int sum =
			rainspotStats.shower * 1 +
			rainspotStats.lightRain * 2 +
			rainspotStats.medium * 3 +
			rainspotStats.heavy * 9;

	// switch if this is rainfall or snowfall or thunderstorm
	switch (pictocode) {
		case HEAVY_RAIN_THUNDERSTORMS_LIKELY:
		case LIGHT_RAIN_THUNDERSTORMS_LIKELY:
		case RAIN_THUNDERSTORMS_LIKELY:
			out = RAIN_TYPE_THUNDERSTORM;
			break;
		case OVERCAST_WITH_MIXTURE_OF_SNOW_AND_RAIN:
		case OVERCAST_WITH_LIGHT_SNOW:
		case OVERCAST_WITH_HEAVY_SNOW:
		case MIXED_WITH_SNOW_SHOWERS:
		case OVERCAST_WITH_SNOW:
			// snow
			if (sum >= 0 && sum < NO_RAIN) {
				out = RAIN_TYPE_SHOWERS_SNOW;
			}
			else if (sum >= NO_RAIN && sum < LIGHT_RAIN) {
				out = RAIN_TYPE_LIGHT_SNOW;
			}
			else if (sum >= LIGHT_RAIN && sum < MEDIUM_RAIN) {
				out = RAIN_TYPE_MEDIUM_SNOW;
			}
			else if (sum >= MEDIUM_RAIN) {
				out = RAIN_TYPE_HEAVY_SNOWFALL;
			}
			break;
		case OVERCAST_WITH_LIGHT_RAIN:
		case OVERCAST_WITH_HEAVY_RAIN:
		case OVERCAST_WITH_RAIN:
		case MIXED_WITH_SHOWERS:
			// rain
			if (sum >= 0 && sum < NO_RAIN) {
				out = RAIN_TYPE_SHOWERS_RAIN;
			}
			else if (sum >= NO_RAIN && sum < LIGHT_RAIN) {
				out = RAIN_TYPE_LIGHT_RAIN;
			}
			else if (sum >= LIGHT_RAIN && sum < MEDIUM_RAIN) {
				out = RAIN_TYPE_MEDIUM_RAIN;
			}
			else if (sum >= MEDIUM_RAIN) {
				out = RAIN_TYPE_HEAVY_RAIN;
			}
			break;
		default:
			// there is no rain exactly at this forecast point, but maybe there
			// is something in the neighborhood.
			if (sum >= NO_RAIN) {
				out = RAIN_TYPE_LOCAL_INTERMITTEND;
			}
			break;
	}

	return out;
}

MeteoblueRainParser::MeteoblueRainParser_PrecipType MeteoblueRainParser::getRainForecastFromMeteoblue(
		std::shared_ptr<org::openapitools::client::model::Inline_response_200> &forecast,
		std::shared_ptr<ConfigurationFile> & configurationFile) {

	MeteoblueRainParser_PrecipType out;

	// future time from configuration
	uint32_t ft = configurationFile->getForecast().futureTime;

	// extract pictocode
	MeteobluePictocode pictocode = std::get<1>(ForecastFinder::getPictocodeMeteoblue(forecast, ft));

	// get rainspot
	std::string rainspot = std::get<1>(ForecastFinder::getRainSpotMeteoblue(forecast, ft));

	out = parse(pictocode, rainspot);

	return out;

}

MeteoblueRainParser::~MeteoblueRainParser() {
	// TODO Auto-generated destructor stub
}


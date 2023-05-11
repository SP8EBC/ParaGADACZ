/*
 * MeteoblueRainParser.h
 *
 *  Created on: May 8, 2023
 *      Author: mateusz
 */

#ifndef METEOBLUERAINPARSER_H_
#define METEOBLUERAINPARSER_H_

#include "Inline_response_200.h"
#include "ConfigurationFile.h"

class MeteoblueRainParser {

private:

	static const int NO_RAIN = 19;		//!<< upper barrier for no precipation
	static const int LIGHT_RAIN = 49;	//!<< upper barrier for light precipation
	static const int MEDIUM_RAIN = 99;	//!<< upper barrier for medium precipation
				// everything above is heavy rain
public:

	/**
	 *	Struct to sum different kinds of meteoblue rainspots
	 */
	struct MeteoblueRainParser_Rs {
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
	enum MeteoblueRainParser_PrecipType {
		RAIN_TYPE_NO_PRECIPATION,	//!<< No rain or snow at all
		RAIN_TYPE_SHOWERS_RAIN,		//!<< rain up to half milimeter
		RAIN_TYPE_SHOWERS_SNOW,		//!<< snow up to half milimeter
		RAIN_TYPE_LIGHT_RAIN,	//!<< rain up to 2mm
		RAIN_TYPE_LIGHT_SNOW,	//!<< snow up to 2mm
		RAIN_TYPE_MEDIUM_RAIN,	//!<< rain up to 5mm
		RAIN_TYPE_MEDIUM_SNOW,	//!<< snow up to 5mm
		RAIN_TYPE_HEAVY_RAIN,		//!<< heavy rainfall
		RAIN_TYPE_HEAVY_SNOWFALL,	//!<< heavy snowfall
		RAIN_TYPE_THUNDERSTORM	//!<< thunderstorm possible
	};

	/**
	 * Helper private method for assembling rain forecast
	 * @param forecast
	 * @return
	 */
	static MeteoblueRainParser_PrecipType putRainForecastFromMeteoblue(
			std::shared_ptr<org::openapitools::client::model::Inline_response_200> & forecast,
			std::shared_ptr<ConfigurationFile> & configurationFile);



	MeteoblueRainParser();
	virtual ~MeteoblueRainParser();
};

#endif /* METEOBLUERAINPARSER_H_ */

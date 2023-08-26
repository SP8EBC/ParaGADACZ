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
#include "MeteobluePictocode.h"

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
		RAIN_TYPE_NO_PRECIPATION,		//!<< No rain or snow at all
		RAIN_TYPE_LOCAL_INTERMITTEND,	//!<< Some precipations in vicinity
		RAIN_TYPE_SHOWERS_RAIN,			//!<< rain up to one milimeter
		RAIN_TYPE_SHOWERS_SNOW,			//!<< snow up to one milimeter
		RAIN_TYPE_LIGHT_RAIN,			//!<< rain up to 3mm
		RAIN_TYPE_LIGHT_SNOW,			//!<< snow up to 3mm
		RAIN_TYPE_MEDIUM_RAIN,			//!<< rain up to 5mm
		RAIN_TYPE_MEDIUM_SNOW,			//!<< snow up to 5mm
		RAIN_TYPE_HEAVY_RAIN,			//!<< heavy rainfall
		RAIN_TYPE_HEAVY_SNOWFALL,		//!<< heavy snowfall
		RAIN_TYPE_LIGHT_RAIN_THUNDER,	//!<< rain up to 3mm + thunderstorm possible
		RAIN_TYPE_MEDIUM_RAIN_THUNDER,	//!<< rain up to 5mm + thunderstorm possible
		RAIN_TYPE_HEAVY_RAIN_THUNDER,	//!<< heavy rainfall + thunderstorm possible
		RAIN_TYPE_LOCAL_THUNDERSTORM,	//!<< Some precipations in vicinity + thunderstorm possible
		RAIN_TYPE_UNKNOWN
	};

	/**
	 * Helper private method for assembling rain forecast
	 * @param forecast
	 * @return
	 */
	static MeteoblueRainParser_PrecipType getRainForecastFromMeteoblue(
			std::shared_ptr<org::openapitools::client::model::Inline_response_200> & forecast,
			std::shared_ptr<ConfigurationFile> & configurationFile);



	MeteoblueRainParser();
	virtual ~MeteoblueRainParser();

protected:

	static MeteoblueRainParser::MeteoblueRainParser_PrecipType parse(
			MeteobluePictocode pictocode,
			std::string rainspot);
};

#endif /* METEOBLUERAINPARSER_H_ */

/*
 * MeteobluePictocode.h
 *
 *  Created on: Apr 30, 2023
 *      Author: mateusz
 */

#ifndef METEOBLUEPICTOCODE_H_
#define METEOBLUEPICTOCODE_H_

typedef enum MeteobluePictocode {

	CLEAR_CLOUDLESS_SKY,
	CLEAR_FEW_CIRRUS,
	CLEAR_WITH_CIRRUS,
	CLEAR_WITH_FEW_LOW_CLOUDS,
	CLEAR_WITH_FEW_LOW_CLOUDS_AND_FEW_CIRRUS,
	CLEAR_WITH_FEW_LOW_CLOUDS_AND_CIRRUS,
	PARTLY_CLOUDY,
	PARTLY_CLOUDY_AND_FEW_CIRRUS,
	PARTLY_CLOUD_AND_CIRRUS,
	MIXED_WITH_SOME_THUNDERSTORM_CLOUDS_POSSIBLE,
	MIXED_WITH_FEW_CIRRUS_SOME_THUNDERSTORM_CLOUDS_POSSIBLE,
	MIXED_WITH_CIRRUS_AND_SOME_THUNDERSTORM_CLOUDS_POSSIBLE,
	CLEAR_BUT_HAZY,
	CLEAR_BUT_HAZY_WITH_FEW_CIRRUS,
	CLEAR_BUT_HAZY_WITH_CIRRUS,
	FOG_LOW_STRATUS_CLOUDS,
	FOG_LOW_STRATUS_CLOUDS_WITH_FEW_CIRRUS,
	FOG_LOW_STRATUS_CLOUDS_WITH_CIRRUS,
	MOSTLY_CLOUDY,
	MOSTLY_CLOUDY_AND_FEW_CIRRUS,
	MOSTLY_CLOUDY_AND_CIRRUS,
	OVERCAST,
	OVERCAST_WITH_RAIN,
	OVERCAST_WITH_SNOW,
	OVERCAST_WITH_HEAVY_RAIN,
	OVERCAST_WITH_HEAVY_SNOW,
	RAIN_THUNDERSTORMS_LIKELY,
	LIGHT_RAIN_THUNDERSTORMS_LIKELY,
	STORM_WITH_HEAVY_SNOW,
	HEAVY_RAIN_THUNDERSTORMS_LIKELY,
	MIXED_WITH_SHOWERS,
	MIXED_WITH_SNOW_SHOWERS,
	OVERCAST_WITH_LIGHT_RAIN,
	OVERCAST_WITH_LIGHT_SNOW,
	OVERCAST_WITH_MIXTURE_OF_SNOW_AND_RAIN,
	WRONG_PICTOCODE
}MeteobluePictocode;

MeteobluePictocode MeteobluePictocode_valueOf(int val);

#endif /* METEOBLUEPICTOCODE_H_ */

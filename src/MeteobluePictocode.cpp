/*
 * MeteobluePictocode.cpp
 *
 *  Created on: Apr 30, 2023
 *      Author: mateusz
 */

#include "MeteobluePictocode.h"

MeteobluePictocode MeteobluePictocode_valueOf(int val) {

	MeteobluePictocode out = WRONG_PICTOCODE;

	switch(val) {
		case 1: out = CLEAR_CLOUDLESS_SKY; break;
		case 2: out = CLEAR_FEW_CIRRUS; break;
		case 3: out = CLEAR_WITH_CIRRUS; break;
		case 4: out = CLEAR_WITH_FEW_LOW_CLOUDS; break;
		case 5: out = CLEAR_WITH_FEW_LOW_CLOUDS_AND_FEW_CIRRUS; break;
		case 6: out = CLEAR_WITH_FEW_LOW_CLOUDS_AND_CIRRUS; break;
		case 7: out = PARTLY_CLOUDY; break;
		case 8: out = PARTLY_CLOUDY_AND_FEW_CIRRUS; break;
		case 9: out = PARTLY_CLOUD_AND_CIRRUS; break;
		case 10: out = MIXED_WITH_SOME_THUNDERSTORM_CLOUDS_POSSIBLE; break;
		case 11: out = MIXED_WITH_FEW_CIRRUS_SOME_THUNDERSTORM_CLOUDS_POSSIBLE; break;
		case 12: out = MIXED_WITH_CIRRUS_AND_SOME_THUNDERSTORM_CLOUDS_POSSIBLE; break;
		case 13: out = CLEAR_BUT_HAZY; break;
		case 14: out = CLEAR_BUT_HAZY_WITH_FEW_CIRRUS; break;
		case 15: out = CLEAR_BUT_HAZY_WITH_CIRRUS; break;
		case 16: out = FOG_LOW_STRATUS_CLOUDS; break;
		case 17: out = FOG_LOW_STRATUS_CLOUDS_WITH_FEW_CIRRUS; break;
		case 18: out = FOG_LOW_STRATUS_CLOUDS_WITH_CIRRUS; break;
		case 19: out = MOSTLY_CLOUDY; break;
		case 20: out = MOSTLY_CLOUDY_AND_FEW_CIRRUS; break;
		case 21: out = MOSTLY_CLOUDY_AND_CIRRUS; break;
		case 22: out = OVERCAST; break;
		case 23: out = OVERCAST_WITH_RAIN; break;
		case 24: out = OVERCAST_WITH_SNOW; break;
		case 25: out = OVERCAST_WITH_HEAVY_RAIN; break;
		case 26: out = OVERCAST_WITH_HEAVY_SNOW; break;
		case 27: out = RAIN_THUNDERSTORMS_LIKELY; break;
		case 28: out = LIGHT_RAIN_THUNDERSTORMS_LIKELY; break;
		case 29: out = STORM_WITH_HEAVY_SNOW; break;
		case 30: out = HEAVY_RAIN_THUNDERSTORMS_LIKELY; break;
		case 31: out = MIXED_WITH_SHOWERS; break;
		case 32: out = MIXED_WITH_SNOW_SHOWERS; break;
		case 33: out = OVERCAST_WITH_LIGHT_RAIN; break;
		case 34: out = OVERCAST_WITH_LIGHT_SNOW; break;
		case 35: out = OVERCAST_WITH_MIXTURE_OF_SNOW_AND_RAIN; break;
		case 36: out = WRONG_PICTOCODE; break;
	}

	return out;
}


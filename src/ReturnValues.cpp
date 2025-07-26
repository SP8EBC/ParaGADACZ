/*
 * ReturnValues.cpp
 *
 *  Created on: Jul 26, 2025
 *      Author: mateusz
 */

#include "ReturnValues.h"

const char * returnValues_toString(returnValues in)
{
	switch (in)
	{
	case OK:									return "OK"; break;
	case NOT_VALID_APRS_PACKET:					return "[NOT_VALID_APRS_PACKET]               "; break;
	case CORRUPTED_APRS_PACKET:					return "[CORRUPTED_APRS_PACKET]               "; break;
	case CALL_AND_SSID_SEPARATION_FAILED:		return "[CALL_AND_SSID_SEPARATION_FAILED]     "; break;
	case SOURCE_CALLSIGN_REGEXP_DOESNT_MATCH:	return "[SOURCE_CALLSIGN_REGEXP_DOESNT_MATCH] "; break;
	case APRS_IS_PATH_EMPTY:					return "[APRS_IS_PATH_EMPTY]                  "; break;
	case DESTINATION_CALL_INVALID:				return "[DESTINATION_CALL_INVALID]            "; break;
	}

	return "?UNKNOWN?";
}



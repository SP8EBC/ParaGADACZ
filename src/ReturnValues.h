/*
 * ReturnValues.h
 *
 *  Created on: 30.10.2018
 *      Author: mateusz
 */

#ifndef RETURNVALUES_H_
#define RETURNVALUES_H_

enum returnValues {
	OK = 0,
	NOT_VALID_APRS_PACKET = -10,
	CORRUPTED_APRS_PACKET = -11,
	CALL_AND_SSID_SEPARATION_FAILED = -12,
	SOURCE_CALLSIGN_REGEXP_DOESNT_MATCH = -13,
	APRS_IS_PATH_EMPTY		= -14,
	DESTINATION_CALL_INVALID	= -15,
	SOURCE_CALL_OR_SSID_INVALID		= -16
};

const char * returnValues_toString(returnValues in);


#endif /* RETURNVALUES_H_ */

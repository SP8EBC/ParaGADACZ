/*
 * PansaAirspaceTypes.h
 *
 *  Created on: Jan 4, 2024
 *      Author: mateusz
 */

#ifndef PANSAAIRSPACETYPES_H_
#define PANSAAIRSPACETYPES_H_

#include <string>
#include <vector>
#include <stdexcept>

typedef enum PansaAirspace_Type {
	AIRSPACE_TRA,
	AIRSPACE_TSA,
	AIRSPACE_ATZ,
	AIRSPACE_D,
	AIRSPACE_R,
	AIRSPACE_P,
	AIRSPACE_ADHOC
}PansaAirspace_Type;

static std::string PansaAirspace_Type_ToString(PansaAirspace_Type type) {
	switch (type) {
		case AIRSPACE_TRA: return "AIRSPACE_TRA";
		case AIRSPACE_TSA: return "AIRSPACE_TSA";
		case AIRSPACE_ATZ: return "AIRSPACE_ATZ";
		case AIRSPACE_D: return "AIRSPACE_D";
		case AIRSPACE_R: return "AIRSPACE_R";
		case AIRSPACE_P: return "AIRSPACE_P";
		case AIRSPACE_ADHOC: return "AIRSPACE_ADHOC";
	}

	return "AIRSPACE_UNKNOWN???";
}

static PansaAirspace_Type PansaAirspace_Type_FromString(std::string _in) {
	if (_in == "TRA" || _in == "\"TRA\"" ) {
		return AIRSPACE_TRA;
	}
	else if (_in == "ATZ" || _in == "\"ATZ\"") {
		return AIRSPACE_ATZ;
	}
	else if (_in == "TSA" || _in == "\"TSA\"") {
		return AIRSPACE_TSA;
	}
	else if (_in == "D" || _in == "\"D\"") {
		return AIRSPACE_D;
	}
	else if (_in == "R" || _in == "\"R\"") {
		return AIRSPACE_R;
	}
	else if (_in == "P" || _in == "\"P\"") {
		return AIRSPACE_P;
	}
	else if (_in == "ADHOC" || _in == "\"ADHOC\"") {
		return AIRSPACE_ADHOC;
	}
	else {
		throw std::runtime_error("Unkown airspace type: " + _in);
	}
}

class PansaAirspace_Reservation {
public:

	PansaAirspace_Reservation() :
		fromTime(0), toTime(0), lowerAltitude(0), upperAltitude(0)
	{

	}

	PansaAirspace_Reservation(
			uint64_t _fromTime, uint64_t _toTime, uint32_t _lowerAltitude, uint32_t _upperAltitude, std::string _unit, std::string _remarks) :
			fromTime(_fromTime), toTime(_toTime), lowerAltitude(_lowerAltitude), upperAltitude(_upperAltitude), unit(_unit), remarks(_remarks)
	{

	}

	PansaAirspace_Reservation(
			uint64_t _fromTime, uint64_t _toTime, uint32_t _lowerAltitude, uint32_t _upperAltitude) :
			fromTime(_fromTime), toTime(_toTime), lowerAltitude(_lowerAltitude), upperAltitude(_upperAltitude)
	{

	}

	uint64_t fromTime;

	uint64_t toTime;

	uint32_t lowerAltitude;

	uint32_t upperAltitude;

	std::string unit;

	std::string remarks;
};

class PansaAirspace_Zone {
public:
	PansaAirspace_Type type;

	std::string designator;

	float centroidLatitudeY;

	float centroidLongitudeX;

	std::vector<PansaAirspace_Reservation> reservations;

	float distanceFromSetpoint;

};


#endif /* PANSAAIRSPACETYPES_H_ */

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
}PansaAirspace_Type;

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
	else {
		throw std::runtime_error("");
	}
}

class PansaAirspace_Reservation {
public:
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

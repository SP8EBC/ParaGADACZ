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

typedef enum PansaAirspace_Type {
	AIRSPACE_TRA,
	AIRSPACE_TSA,
	AIRSPACE_ATZ,
	AIRSPACE_D,
	AIRSPACE_R,
	AIRSPACE_P,
};

class PansaAirspace_Reservation {

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

	float centroidX;

	float centroidY;

	std::vector<PansaAirspace_Reservation> reservations;

	float distanceFromSetpoint;

};


#endif /* PANSAAIRSPACETYPES_H_ */

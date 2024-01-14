/*
 * PansaAirspace.h
 *
 *  Created on: Jan 3, 2024
 *      Author: mateusz
 */
#ifdef PANSA_AIRSPACE

#ifndef PANSAAIRSPACE_H_
#define PANSAAIRSPACE_H_

#include "PansaAirspaceTypes.h"

#include <string>
#include <map>

class PansaAirspace {
private:
	static float ConvertAltitudeStr(std::string _str);

	std::string postgresUsername;

	std::string postgresPassword;

	std::string postgresDb;

	std::string connectionStr;

	/**
	 * Map of all reservation downloaded from the system
	 */
	std::map<std::string, PansaAirspace_Zone> reservations;

	/**
	 * If there is at least one airspace which is reserved more
	 * than one time per day
	 */
	bool hasDoubleReservation;
public:
	PansaAirspace(std::string psqlUsername, std::string psqlPassword, std::string psqlDb);
	virtual ~PansaAirspace();

	void downloadAroundLocation(float lat, float lon, int radius, bool dumpSqlQuery);

	const std::map<std::string, PansaAirspace_Zone>& getReservations() const {
		return reservations;
	}
};

#endif /* PANSAAIRSPACE_H_ */

#endif

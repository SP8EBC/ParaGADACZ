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
#include <memory>

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

	/**
	 * Downloads all reservations for airspace elements located within an area defined
	 * by coordinates and radius, results are stored in internal map thus this method
	 * has side effects.
	 * @param lat
	 * @param lon
	 * @param radius
	 * @param dumpSqlQuery
	 * @return how many reservations were found
	 */
	int downloadAroundLocation(float lat, float lon, int radius, bool dumpSqlQuery);

	/**
	 * Download all reservation for explicitly given airspace designator
	 * @param designator
	 * @return vector of all reservations found or an empty one if no reservations exists
	 */
	std::vector<std::shared_ptr<PansaAirspace_Reservation>> downloadForDesginator(std::string designator, bool dumpSqlQuery);

	const std::map<std::string, PansaAirspace_Zone>& getReservations() const {
		return reservations;
	}
};

#endif /* PANSAAIRSPACE_H_ */

#endif

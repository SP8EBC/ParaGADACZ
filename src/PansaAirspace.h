/*
 * PansaAirspace.h
 *
 *  Created on: Jan 3, 2024
 *      Author: mateusz
 */
#ifdef PANSA_AIRSPACE_ENABLED

#ifndef PANSAAIRSPACE_H_
#define PANSAAIRSPACE_H_

#include "PansaAirspaceTypes.h"

#include <string>
#include <map>
#include <memory>
#include <vector>

class PansaAirspace_ResultsAroundPoint {
public:
	std::string pointName;						//!< Name of this point, should be a filename with an announcement
	float lat;
	float lon;
	int radius;								//!< In meters
	std::map<std::string, PansaAirspace_Zone> reservations;	//!< All zones and it's reservations detected for specified area
};

class PansaAirspace_ResultsForDesignator {
public:
	PansaAirspace_Type type;
	std::string designator;
	bool sayAltitude;
	bool sayTime;
	std::vector<std::shared_ptr<PansaAirspace_Reservation>> reservations;	//!< reservations
};

class PansaAirspace {
private:
	static float ConvertAltitudeStr(std::string _str);

	std::string postgresUsername;

	std::string postgresPassword;

	std::string postgresDb;

	std::string connectionStr;

	/**
	 * Map of all reservation downloaded from the system.
	 */
	std::vector<std::shared_ptr<PansaAirspace_ResultsAroundPoint>> reservationsAroundPoints;

	std::vector<std::shared_ptr<PansaAirspace_ResultsForDesignator>> reservationsForZones;

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
	 * @param name of this point to check. by default a audio anouncement sample filename shall be used here
	 * @param lat
	 * @param lon
	 * @param radius
	 * @param dumpSqlQuery
	 * @return how many reservations were found
	 */
	int downloadAroundLocation(std::string name, float lat, float lon, int radius, bool dumpSqlQuery, float maximumLowerAltitude);

	/**
	 * Download all reservation for explicitly given airspace designator
	 * @param designator of an airspace to check
	 * @param sayAltitude this parameter is only copied to a class holdind the result. is used later by an assembler
	 * @param sayTime this parameter is only copied to a class holdind the result. is used later by an assembler
	 * @return vector of all reservations found or an empty one if no reservations exists
	 */
	int downloadForDesginator(std::string designator, bool sayAltitude, bool sayTime, bool dumpSqlQuery);

	const std::vector<std::shared_ptr<PansaAirspace_ResultsAroundPoint>>& getReservationsAroundPoints() const {
		return reservationsAroundPoints;
	}

	const std::vector<std::shared_ptr<PansaAirspace_ResultsForDesignator> >& getReservationsForZones() const {
		return reservationsForZones;
	}
};

#endif /* PANSAAIRSPACE_H_ */

#endif

/*
 * PansaAirspace.cpp
 *
 *  Created on: Jan 3, 2024
 *      Author: mateusz
 */

#include "PansaAirspace.h"

#ifdef PANSA_AIRSPACE_ENABLED
#include <pqxx/pqxx>
#include <pqxx/connection>

#include <sstream>
#include <iostream>
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

#include <ctemplate/template.h>

std::string query = R"( select
	json_body->'properties'->>'designator'::text as designator,
	json_body->'properties'->>'airspaceElementType' as airspaceElementType,
	json_array_elements(json_body->'properties'->'centroid')->'x' as centroidLon,
	json_array_elements(json_body->'properties'->'centroid')->'y' as centroidLat,
	ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography),
	extract (epoch from (airspaceReservation->>'startDate')::timestamp) as epochFrom,
	extract (epoch from (airspaceReservation->>'endDate')::timestamp) as epochTo,
	airspaceReservation->>'lowerAltitude' as lowerAltitude,
	airspaceReservation->>'upperAltitude' as upperAltitude,
	airspaceReservation->>'unit' as unit,
	airspaceReservation->>'remarks' as remarks,
	
	geography,
	json_body
from (
	select 
		json_array_elements(json_body->'properties'->'airspaceReservations') as airspaceReservation,
		geography,
		json_body
	from
	(
		select
		ST_GeomFromGeoJSON(value->'geometry')::geography as geography,
		value as json_body
		from
		json_array_elements((select content::json from http_get('https://airspace.pansa.pl/map-configuration/aup')))
	) as cycki 
) as dupa
WHERE ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography) < 70000 ORDER by st_distance;)";



float PansaAirspace::ConvertAltitudeStr(std::string _str) {

	const std::size_t strSize = _str.length();

	if (strSize != 3 && strSize != 4) {
		throw std::runtime_error("");
	}

	// ground -> "GND"
	if (_str[0] == 'G') {
		return 0;
	}
	else if(_str[0] == 'A') {
		// substring with altitude value itself
		const std::string _value = _str.substr(1, strSize - 1);

		// and here as an integer multipled by 100
		const int _valueAsInt = 100 * std::stoi(_value);

		return (int)(::roundf(_valueAsInt * 0.3048f));
	}
	else if (_str[0] == 'F') {
		// we should really consider QNH pressure here, but as for now
		// just keep it simple

		// substring with altitude value itself
		const std::string _value = _str.substr(1, strSize - 1);

		// and here as an integer multipled by 100
		const int _valueAsInt = 100 * std::stoi(_value);

		return (int)(::roundf(_valueAsInt * 0.3048f));
	}
	else {
		throw std::runtime_error("");
	}
}

PansaAirspace::PansaAirspace(std::string psqlUsername, std::string psqlPassword, std::string psqlDb) {
	hasDoubleReservation = false;

	postgresDb = psqlDb;
	postgresUsername = psqlUsername;
	postgresPassword = psqlPassword;

	std::stringstream ss;
	ss << "user=" << postgresUsername << " host=localhost password=" << postgresPassword << " dbname=" << postgresDb;

	connectionStr = ss.str();
}

PansaAirspace::~PansaAirspace() {
	// TODO Auto-generated destructor stub
}

int PansaAirspace::downloadAroundLocation(std::string name, float lat, float lon, int radius, bool dumpSqlQuery, float maximumLowerAltitude) {

	int results = 0;

	std::string queryStr;

	// result for single airspace
	std::shared_ptr<PansaAirspace_ResultsAroundPoint> singleResult = std::make_shared<PansaAirspace_ResultsAroundPoint>();

	singleResult->pointName = name;
	singleResult->lon = lon;
	singleResult->lat = lat;
	singleResult->radius = radius;

	ctemplate::TemplateDictionary dict("pansa_around_point");

    dict.SetFormattedValue("longitude", "%.4f", lon);
    dict.SetFormattedValue("latitude", "%.4f", lat);
    dict.SetIntValue("distance", radius);

    ctemplate::ExpandTemplate("pansa_around_point.tpl", ctemplate::DO_NOT_STRIP, &dict, &queryStr);

    if (dumpSqlQuery) {
    	SPDLOG_DEBUG("PostgreSQL used to download data from PANSA api \r\n{}", queryStr);
    }

	//pqxx::connection c{"user=postgres host=localhost password=dupajasia123 dbname=airspace"};
    pqxx::connection c{connectionStr};
	pqxx::work txn{c};

	pqxx::result res = txn.exec(queryStr);

	  for (auto row : res) {
		  PansaAirspace_Zone zone;

		  std::string designator = std::string(row["designator"].c_str());
		  std::string airspaceType = std::string(row["airspace_element_type"].c_str());
		  float centroidLon = row["centroid_lon"].as<float>();
		  float centroidLat = row["centroid_lat"].as<float>();
		  float distanceInMeters = row["ST_Distance"].as<float>();
		  uint64_t epochFrom = (uint64_t)row["epoch_from"].as<double>();
		  uint64_t epochTo = (uint64_t)row["epoch_to"].as<double>();
		  std::string unit = std::string(row["unit"].c_str());
		  std::string remarks = std::string(row["remarks"].c_str());

		  const std::string lowerAltitudeStr = row["lower_altitude"].as<std::string>();
		  int lowerAltitude = PansaAirspace::ConvertAltitudeStr(lowerAltitudeStr);

		  const std::string upperAltitudeStr = row["upper_altitude"].as<std::string>();
		  int upperAltitude = PansaAirspace::ConvertAltitudeStr(upperAltitudeStr);


		  PansaAirspace_Type type = PansaAirspace_Type_FromString(airspaceType);

		  if (lowerAltitude > maximumLowerAltitude) {
			  SPDLOG_WARN("airspace with designator {} has too high lowerAltitude {} and will be ignored", lowerAltitude, maximumLowerAltitude);
			  continue;
		  }


		  SPDLOG_INFO(	"designator: {}, distanceInMeters: {}, lowerAltitudeStr: {}, upperAltitudeStr: {}, lowerAltitude: {}, upperAltitude: {}",
				  	    designator, (int)::roundf(distanceInMeters), lowerAltitudeStr, upperAltitudeStr, lowerAltitude, upperAltitude);

		  std::shared_ptr<PansaAirspace_Reservation> reserv =
		  				  std::make_shared<PansaAirspace_Reservation>(epochFrom, epochTo, lowerAltitude, upperAltitude);

		  reserv->unit = unit;
		  reserv->remarks = remarks;

		  std::map<std::string, PansaAirspace_Zone>::iterator previousRsrv = singleResult->reservations.find(designator);

		  results++;

		  // if at least one reservation for this zone has been found already
		  if (previousRsrv != singleResult->reservations.end()) {

			  // get precious object
			  PansaAirspace_Zone & existing = previousRsrv->second;

			  existing.reservations.push_back(reserv);

			  hasDoubleReservation = true;
		  }
		  else {
			  // this is a new one
			  PansaAirspace_Zone newOne;

			  newOne.centroidLatitudeY = centroidLat;
			  newOne.centroidLongitudeX = centroidLon;
			  newOne.designator = designator;
			  newOne.distanceFromSetpoint = distanceInMeters;
			  newOne.type = type;
			  newOne.reservations.push_back(reserv);

			  singleResult->reservations.insert(std::pair(designator, newOne));

		  }
	  }

	  txn.commit();

		// modification needed to have this compilable and working with libpqxx-7.8t64 in Xubuntu 24.10
		// New version will disconnect a connection in the destructor. Not sure what the older one will do
		//c.disconnect();

	  if (singleResult->reservations.size() == 0) {
		  SPDLOG_WARN("No airspace reservations have been found. It is really OK??");
	  }

	  reservationsAroundPoints.push_back(singleResult);

	  return results;
}

int PansaAirspace::downloadForDesginator(
		std::string designator, bool sayAltitude, bool sayTime, bool dumpSqlQuery) {

	int out = 0;

	// single result, all reservations for this designator
	std::shared_ptr<PansaAirspace_ResultsForDesignator> singleResult = std::make_shared<PansaAirspace_ResultsForDesignator>();
	singleResult->designator = designator;
	singleResult->sayAltitude = sayAltitude;
	singleResult->sayTime = sayTime;

	std::string queryStr;

	PansaAirspace_Type type;

	ctemplate::TemplateDictionary dict("pansa_explicit_designator");

    dict.SetValue("desgiantor", designator);

    ctemplate::ExpandTemplate("pansa_explicit_designator.tpl", ctemplate::DO_NOT_STRIP, &dict, &queryStr);

    SPDLOG_INFO("Looking for active reservations for airspace designator {}", designator);

    if (dumpSqlQuery) {
    	SPDLOG_DEBUG("PostgreSQL used to download data from PANSA api \r\n{}", queryStr);
    }

    pqxx::connection c{connectionStr};
	pqxx::work txn{c};

	pqxx::result res = txn.exec(queryStr);

	for (auto row : res) {
		out++;
		std::string designator = std::string(row["designator"].c_str());
		std::string airspaceType = std::string(row["airspace_element_type"].c_str());
		float epochFrom = row["epoch_from"].as<float>();
		float epochTo = row["epoch_to"].as<float>();

		type = PansaAirspace_Type_FromString(airspaceType);

		const std::string lowerAltitudeStr = row["lower_altitude"].as<std::string>();
		int lowerAltitude = PansaAirspace::ConvertAltitudeStr(lowerAltitudeStr);

		const std::string upperAltitudeStr = row["upper_altitude"].as<std::string>();
		int upperAltitude = PansaAirspace::ConvertAltitudeStr(upperAltitudeStr);

		SPDLOG_INFO(	"airspaceType: {}, lowerAltitudeStr: {}, upperAltitudeStr: {}, lowerAltitude: {}, upperAltitude: {}",
					airspaceType, lowerAltitudeStr, upperAltitudeStr, lowerAltitude, upperAltitude);

		std::shared_ptr<PansaAirspace_Reservation> reserv =
			  std::make_shared<PansaAirspace_Reservation>(epochFrom, epochTo, lowerAltitude, upperAltitude);

		singleResult->reservations.push_back(reserv);
	}

	txn.commit();

	// modification needed to have this compilable and working with libpqxx-7.8t64 in Xubuntu 24.10
	// New version will disconnect a connection in the destructor. Not sure what the older one will do
	//c.disconnect();

	singleResult->type = type;
	reservationsForZones.push_back(singleResult);
	//return std::make_pair(type, out);
	return out;
}

#endif


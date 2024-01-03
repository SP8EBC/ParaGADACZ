/*
 * PansaAirspace.cpp
 *
 *  Created on: Jan 3, 2024
 *      Author: mateusz
 */

#include "PansaAirspace.h"

#ifdef PANSA_AIRSPACE
#include <pqxx/pqxx>
#include <pqxx/connection>

#include <string>

#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

std::string query = R"( select
  json_body->'properties'->>'designator'::text as designator,
   ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography),
     geography,
  json_array_elements(json_body->'properties'->'airspaceReservations') as airspaceReservation,
  json_body
 from
  (
  select
   ST_GeomFromGeoJSON(value->'geometry')::geography as geography,
   value as json_body
  from
   json_array_elements((select content::json from http_get('https://airspace.pansa.pl/map-configuration/aup')))
) as cycki WHERE ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography) < 70000 ORDER by st_distance;)";

PansaAirspace::PansaAirspace() {
	// TODO Auto-generated constructor stub

}

PansaAirspace::~PansaAirspace() {
	// TODO Auto-generated destructor stub
}

void PansaAirspace::connect() {
	// https://stackoverflow.com/questions/7763546/connection-to-postgres-database-using-libpqxx
	  pqxx::connection c{"user=postgres host=localhost password=dupajasia123 dbname=airspace"};
	  pqxx::work txn{c};

	  pqxx::result res = txn.exec(query);

	  for (auto row : res) {
		  SPDLOG_DEBUG("designator: {}", row["designator"].c_str());
	  }

	  txn.commit();

	  c.disconnect();
}

void PansaAirspace::disconnect() {
}

#endif


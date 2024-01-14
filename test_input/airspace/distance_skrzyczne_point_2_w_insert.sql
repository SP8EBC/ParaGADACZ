 INSERT INTO airspace(
	 designator, airspace_element_type, centroid_lon, centroid_lat, st_distance, epoch_from, epoch_to, lower_altitude, upper_altitude, unit, remarks, geography, "json_body")
 select
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
WHERE ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography) < 470000 ORDER by designator
RETURNING *;

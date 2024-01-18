SELECT
	json_body->'properties'->>'designator'::text as designator,
	json_body->'properties'->>'airspaceElementType' as airspace_element_type,
	json_array_elements(json_body->'properties'->'centroid')->'x' as centroid_lon,
	json_array_elements(json_body->'properties'->'centroid')->'y' as centroid_lat,
	ST_Distance(geography,  ST_SetSRID(ST_MakePoint({{longitude}}, {{latitude}}), 4326)::geography),
	extract (epoch from (airspaceReservation->>'startDate')::timestamp) as epoch_from,
	extract (epoch from (airspaceReservation->>'endDate')::timestamp) as epoch_to,
	airspaceReservation->>'lowerAltitude' as lower_altitude,
	airspaceReservation->>'upperAltitude' as upper_altitude,
	airspaceReservation->>'unit' as unit,
	airspaceReservation->>'remarks' as remarks,
	
	geography,
	json_body
FROM (
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
WHERE
ST_Distance(geography,  ST_SetSRID(ST_MakePoint({{longitude}}, {{latitude}}), 4326)::geography) < {{distance}} 
ORDER BY 
st_distance;

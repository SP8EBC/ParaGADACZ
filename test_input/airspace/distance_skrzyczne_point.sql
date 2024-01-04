 select
  json_body->'properties'->>'designator'::text as designator,
  json_body->'properties'->>'airspaceElementType' as airspaceElementType,
	json_array_elements(json_body->'properties'->'centroid')->'x' as centroidLon,
	json_array_elements(json_body->'properties'->'centroid')->'y' as centroidLat,
   ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography),
     json_array_elements(json_body->'properties'->'airspaceReservations') as airspaceReservation,
   extract (epoch from (airspaceReservation->>'startDate')::timestamp),
   geography,
  json_body
 from
  (
  select
   ST_GeomFromGeoJSON(value->'geometry')::geography as geography,
   value as json_body
  from
   json_array_elements((select content::json from http_get('https://airspace.pansa.pl/map-configuration/aup')))
) as cycki WHERE ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography) < 170000 ORDER by st_distance;
 select
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
) as cycki WHERE ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography) < 70000 ORDER by st_distance;
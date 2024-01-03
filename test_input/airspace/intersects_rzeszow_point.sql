 select
  json_body->'properties'->>'designator'::text as designator,
   ST_Intersects(geography,  ST_SetSRID(ST_MakePoint(22.0437, 49.9875), 4326)::geography),
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
) as cycki ORDER by st_intersects;
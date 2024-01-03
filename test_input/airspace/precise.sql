select 
 json_body->'properties'->>'airspaceElementType'::text as airspaceElementType,
 json_body->'properties'->>'designator'::text as designator,
 ST_MakePoint((json_body->'properties'->'centroid'->0->>'x')::float,
 (json_body->'properties'->'centroid'->0->>'y')::float,
 (json_body->'properties'->'centroid'->0->>'z')::float,
 4326) as centroid,
 (json_body->'properties'->>'activeH24')::boolean as activeH24,
 (json_body->'properties'->>'fbz')::boolean as fbz,
 json_body->'properties'->>'fuaStatus'::text as fuaStatus,
 json_body->'properties'->>'section'::text as section,
 (airspaceReservation->>'startDate')::timestamp as startDate,
 (airspaceReservation->>'endDate')::timestamp as endDate
from
 (
 select
  geography,
  json_body,
  json_array_elements(json_body->'properties'->'airspaceReservations') as airspaceReservation
 from
  (
  select
   ST_GeomFromGeoJSON(value->'geometry')::geography as geography,
   value as json_body
  from
   json_array_elements((select content::json from http_get('https://airspace.pansa.pl/map-configuration/aup')))
) as cycki
 where
  ST_Distance(geography,  ST_SetSRID(ST_MakePoint(19.0318, 49.6852), 4326)::geography) < 70000
  -- nautical miles in meters
)as dupa;
-- where
--  (airspaceReservation->>'startDate')::timestamptz >= now()
--  and (airspaceReservation->>'endDate')::timestamptz <= now();
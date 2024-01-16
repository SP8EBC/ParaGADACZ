 select
	json_body->'properties'->>'designator'::text as designator,
	json_body->'properties'->>'airspaceElementType' as airspaceElementType,
	extract (epoch from (airspaceReservation->>'startDate')::timestamp) as epochFrom,
	extract (epoch from (airspaceReservation->>'endDate')::timestamp) as epochTo,
	airspaceReservation->>'lowerAltitude' as lowerAltitude,
	airspaceReservation->>'upperAltitude' as upperAltitude
from (
	select
		json_body,
		json_array_elements(json_body->'properties'->'airspaceReservations') as airspaceReservation
	from
	(
		select
		value as json_body
		from
		json_array_elements((select content::json from http_get('https://airspace.pansa.pl/map-configuration/aup')))
	) as cycki 
) as dupa
WHERE json_body->'properties'->>'designator'::text = 'EPTS6A';

--- this is used

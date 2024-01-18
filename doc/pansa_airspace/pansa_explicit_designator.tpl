 select
	json_body->'properties'->>'designator'::text as designator,
	json_body->'properties'->>'airspaceElementType' as airspace_element_type,
	extract (epoch from (airspaceReservation->>'startDate')::timestamp) as epoch_from,
	extract (epoch from (airspaceReservation->>'endDate')::timestamp) as epoch_to,
	airspaceReservation->>'lowerAltitude' as lower_altitude,
	airspaceReservation->>'upperAltitude' as upper_altitude
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
WHERE json_body->'properties'->>'designator'::text = '{{desgiantor}}';


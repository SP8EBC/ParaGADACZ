select
	value->'type' as type,
	value->'properties'->'airspaceElementType' as airspaceElementType,
	value->'properties'->'designator' as designator,
	value->'properties'->'centroid' as centroid,
	value->'properties'->'popupHTML' as popupHTML,
	value->'properties'->'activeH24' as activeH24,
	value->'properties'->'section' as section_name,
	value->'properties'->'fbz' as fbz,
	value->'properties'->'fuaStatus' as fuaStatus,
	value->'properties'->'section' as section,
	value->'properties'->'airspaceReservations' as airspaceReservations,
	ST_GeomFromGeoJSON(value->'geometry') as geometry
from
	json_array_elements((select content::json from http_get('https://airspace.pansa.pl/map-configuration/aup')));
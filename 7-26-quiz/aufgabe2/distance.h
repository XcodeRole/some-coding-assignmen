#pragma once

/**
 * Berechnet die Distanz zwischen zwei Geokoordinaten
 *
 * @param lat1 Breitengrad der ersten Geokoordinate
 * @param lon1 Längengrad der ersten Geokoordinate
 * @param lat2 Breitengrad der zweiten Geokoordinate
 * @param lon2 Längengrad der zweiten Geokoordinate
 * @return Distanz zwischen den beiden Koordinaten
 */
double geo_distance(double lat1, double lon1, double lat2, double lon2);

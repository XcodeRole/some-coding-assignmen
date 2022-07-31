#include "distance.h"

#include <cmath>

// Gradmass nach Bogenmass
double to_radian(double degrees) {
     return degrees * M_PI / 180.0;
}

// based on https://gis.stackexchange.com/q/171573
double geo_distance(double lat1, double lon1, double lat2, double lon2) {
    double earth_radius = 6373.0;

    lat1 = to_radian(lat1);
    lon1 = to_radian(lon1);
    lat2 = to_radian(lat2);
    lon2 = to_radian(lon2);

    auto dlon = lon2 - lon1;
    auto dlat = lat2 - lat1;

    auto a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
    auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return earth_radius * c;
}

#include <stdio.h>      /* printf */
#include <math.h>       /* sin */
#include <airport.h>

#define PI 3.14159265
#define deg2rad(a) (a * 3.14159265/180.0)

using namespace std;

airport_base::airport_base() {
    numAirports = 0;
}

airport_base::airport_base(int size) {
    numAirports = size;
    ids.resize(size);
    cities.resize(size);
    names.resize(size);
    alias.resize(size);
    lats.resize(size);
    longs.resize(size);
}

double airport_base::get_distance(int a_Id, int b_Id) {
    double R = 6367.4447;    // radius of the earth according to wolfram alpha and Siri (who used wolfram)
    double lat1 = deg2rad(lats[a_Id]);
    double lat2 = deg2rad(lats[b_Id]);
    double lon1 = deg2rad(longs[a_Id]);
    double lon2 = deg2rad(longs[b_Id]);

    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;

    double u = sin(dlat/2);
    double v = sin(dlon/2);

    double a = u*u + cos(lat1) * cos(lat2) * v*v;
    double c = 2.0 * atan2(sqrt(a), sqrt(1-a)) ;
    double d = R * c;

    return d;
}
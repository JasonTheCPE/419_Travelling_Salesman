#include <stdio.h>
#include <math.h>       /* sin */

#define Deg2Rad(a) (a * 3.14159265/180.0)

double get_distance(double lat1_a, double lat2_a, double lon1_b, double lon2_b) {
    double R = 6367.4447;    // radius of the earth according to wolfram alpha and Siri (who used wolfram)
    double lat1 = Deg2Rad(lat1_a);
    double lat2 = Deg2Rad(lat2_a);
    double lon1 = Deg2Rad(lon1_b);
    double lon2 = Deg2Rad(lon2_b);

    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;

    double u = sin(dlat/2);
    double v = sin(dlon/2);

    double a = u*u + cos(lat1) * cos(lat2) * v*v;
    double c = 2.0 * atan2(sqrt(a), sqrt(1-a)) ;
    double d = R * c;

    return d;
}

int main(void) {
    printf("Answer: 3996.484. \nResult: %lf\n", get_distance(33.9425, 20.8987, 118.4081, 156.4305));
    
    const char* target = "../airports.dat";
   airport_base:airport_base ab;
   ParseAirports(target, &ab);

   for(int i = 0; i < ab.numAirports; i++) {
      cout << "id: " << ab.ids[i] << endl;
      cout << "city: " << ab.cities[i] << endl;
      cout << "alias: " << ab.alias[i] << endl;
      cout << "lat: " << ab.lats[i] << endl;
      cout << "lon: " << ab.longs[i] << endl << endl;
   }
   return 0;
    return 0;
}

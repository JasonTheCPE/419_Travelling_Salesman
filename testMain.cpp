#include <stdio.h>
#include "airprt.h"
#include "AirParse.h"
#include "tsp.h"

using namespace std;

int main(void) {
   printf("Answer: 3996.484 \nResult: %lf\n", get_distance(33.9425, 20.8987, 118.4081, 156.4305));
    
   const char* airFile = "../airports.dat";
   const char* rtFile = "../routes.dat";
   map<int,airport> airports;
   map<string,city> cities;


   GetAllInfo(rtFile, airFile, &cities, &airports);

/*
   for(int i = 0; i < ab.numAirports; i++) {
      cout << "id: " << ab.ids[i] << endl;
      cout << "city: " << ab.cities[i] << endl;
      cout << "alias: " << ab.alias[i] << endl;
      cout << "lat: " << ab.lats[i] << endl;
      cout << "lon: " << ab.longs[i] << endl << endl;
   }

   for(int i = 0; i < rt.numRoutes; i++) {
      cout << "sourceid: " << rt.sourceID[i] << endl;
      cout << "destid: " << rt.destID[i] << endl;
      cout << "sourcealias: " << rt.sourceAlias[i] << endl;
      cout << "destalias: " << rt.destAlias[i] << endl << endl;
   }
*/
   return 0;
}

#include <stdio.h>
#include <limits>
#include "airport.h"
#include "AirParse.h"
#include "tsp.h"

using namespace std;

int main(int argc, const char *argv[]) {
   map<int,airport> airports;
   map<string,city> cities;
   vector<string> cityNames;
   vector<route> routes;
   vector<vector<vector<int> > > airMap;
   int routeNum;

   if(argc != 3) {
      cout << "Usage: partsp <routes.dat> <airports.dat>" << endl;
      exit(EXIT_FAILURE);
   }

   GetAllInfo(argv[1], argv[2], &cities, &cityNames, &airports, &routeNum);

   routes.resize(routeNum);
   airMap.resize(cities.size());

   int cityMapSize = cities.size();
   vector<vector<double> > cityMap(cityMapSize,
          vector<double>(cityMapSize, numeric_limits<double>::max()));
   
   //cout << "cityNames: " << cityNames.size() << endl;
   FillRouteVector(routes, airMap, cityMap, cities, cityNames, airports);
   
   createFloydTable(cities.size(), cityMap, airMap);
   //route = createRoute(numCities, TODO_Slo_index_in_table, TODO, airMap);

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

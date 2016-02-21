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
   
#ifdef DEBUG
   cout << "Parsing Input files\n";
#endif
   GetAllInfo(argv[1], argv[2], cities, cityNames, airports, routeNum);

   routes.resize(routeNum);
   airMap.resize(cities.size());

   int cityMapSize = cities.size();
   vector<vector<double> > cityMap(cityMapSize,
          vector<double>(cityMapSize, numeric_limits<double>::max()));
   
   FillRouteVector(routes, airMap, cityMap, cities, cityNames, airports);

#ifdef DEBUG 
   for(int i = 0; i < cityMapSize; ++i) {
      cout << "City: " << cityNames[i] << endl;
      vector<int> arpts = cities[cityNames[i]].containedAirportIDs;
      for(int j = 0; j < arpts.size(); ++j) {
         cout << "Airport: " << arpts[j] << endl;
         vector<int> outgoing = airports[arpts[j]].outgoingIDs;
         for(int k = 0; k < outgoing.size(); ++k) {
            cout << "Outgoing: " << outgoing[k] << " - City: " << airports[outgoing[k]].cityName << endl;
         }
      }
   }
#endif

#ifdef DEBUG 
   cout << "Constructing Floyd Table\n";
#endif
   createFloydTable(cities.size(), cityMap, airMap);

#ifdef DEBUG 
   cout << "Creating Approximate Route\n";
#endif
   vector<int> rts = createRoute(cityMapSize, airports[5768].cityID, cityMap, airMap);

#ifdef DEBUG 
   cout << "Writing results.csv\n";
#endif
   printToCSV("results.csv", rts, routes, airports, cityNames); 

   return 0;
}

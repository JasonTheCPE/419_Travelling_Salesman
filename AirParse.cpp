
#include <iostream>
#include <string>
#include "AirParse.h"
#include "airport.h"
#include "tsp.h"

using namespace std;

/*
 * Fills out the maps for airports and cities.
 */
void GetAllInfo(const char* routeFilename, const char* airportsFilename,
                std::map<std::string, city> &cities,
                std::vector<std::string> &cityNames,
                std::map<int, airport> &airports, int &routeNum) {

   GetRouteInfo(routeFilename, airports, routeNum);
   GetCityAirportsInfo(airportsFilename, cities, cityNames, airports);
}

/*
 * Takes a the name of a file containing airport information and an empty map
 * with city name keys and city struct values as well as another populated map
 * with airport ID keys and airprt struct values. Then populates both maps by
 * reading the file filling in the information for:
 *
 * In the cities map...
 * vector<int> airportIDs
 *
 * In the airports map...
 * double lat
 * double lon
 */
void GetCityAirportsInfo(const char* airportsFilename,
                         std::map<std::string, city> &cities,
                         std::vector<std::string> &cityNames,
                         std::map<int, airport> &airports) {

   ifstream inFile(airportsFilename);
   stringstream ss, conv;
   string line;
   string delimiters = ",\"";
   //int numAirports = 0;
   int numCities = 0;
   bool skip;
   int index;
   size_t prev, pos;

   int tempInt;
   double tempDbl;
   string cityName;

   if(!inFile) {
      cerr << "Problem reading from " << airportsFilename << endl;
   }
      
   while(getline(inFile, line)) 
   {
      ss.clear();
      ss.str("");
      ss.str(line);
      prev = index = 0;
      skip = false;

      while ((pos = line.find_first_of(delimiters, prev)) != string::npos && !skip)
      {

         if (pos > prev) {
            switch(index) {
               case ID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> tempInt;
                  if(airports.find(tempInt) == airports.end()) {
                     // Airport has no routes from in the route table, skip rest of line
                     skip = true;
                  } else {
                     // Airport has routes from, continue gathering info
                  }
                  break;
               case CITYNAME_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  cityName = line.substr(prev, pos - prev);
                  airports[tempInt].cityID = cityNames.size();
                  airports[tempInt].cityName = string(cityName);
                  if(cities.find(cityName) == cities.end()) {
                     // Add cities to the name list only once
                     cityNames.push_back(cityName);
                  }

                  cities[cityName].containedAirportIDs.push_back(tempInt);
                  break;
               case LAT_INDEX:
                  //cout << "LAT: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempDbl;
                  airports[tempInt].lat = tempDbl;
                  break;
               case LON_INDEX:
                  //cout << "LON: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempDbl;
                  airports[tempInt].lon = tempDbl;
                  break;
               default:
                  break;
            }
            ++index;
         }
         prev = pos + 1;
      }
   }
   
   cout << "Cities: " << cities.size() << endl;
}

/*
 * Takes a the name of a file containing routes and an empty map with airport
 * ID keys and airprt struct values. Then populates the map by reading the
 * file filling in the information for:
 *
 * vector<int> outgoingDests
 * string alias
 */
void GetRouteInfo(const char* routeFilename,
                  std::map<int, airport> &airports, int &routeNum) {
   ifstream inFile(routeFilename);
   stringstream ss, conv;
   string line;
   string delimiters = ",";
   int index;
   size_t prev, pos;

   int sourceID, destID;
   string tempStr;

   routeNum = 0;

   if(!inFile) {
      cerr << "Problem reading from " << routeFilename<< endl;
   }
      
   while(getline(inFile, line)) 
   {
      ss.clear();
      ss.str("");
      ss.str(line);
      prev = index = 0;

      while ((pos = line.find_first_of(delimiters, prev)) != string::npos)
      {
         if (pos > prev) {
            switch(index) {
               case SOURCEALIAS_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  // Grab the alias for assignment in the later steps
                  tempStr = line.substr(prev, pos - prev);
                  break;
               case SOURCEID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> sourceID;

                  //TODO ERROR CHECK
                  airports[sourceID].alias = tempStr;
                  break;
               case DESTID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> destID;
                  // Add outgoing route information to each airport
                  airports[sourceID].outgoingIDs.push_back(destID);
                  break;
               default:
                  break;
            }
            ++index;
         }
         prev = pos + 1;
      }
      ++routeNum;
   }
   
   cout << "Airports: " << airports.size() << endl;
   cout << "Routes: " << routeNum << endl;
}

/*
 * [from][to]
 */
void FillRouteVector(std::vector<route> &routes,
                std::vector<std::vector<std::vector<int> > > &airMap,
                std::vector<std::vector<double> > &cityMap,
                std::map<std::string, city> &cities,
                std::vector<std::string> &cityNames,
                std::map<int, airport> &airports) {

   int rtIndex = 0;
   for(int cityIndex = 0; cityIndex < cityNames.size(); ++cityIndex) {
//cout << "cityIndex: " << cityIndex << endl;
      vector<int> cityAirports = cities[cityNames[cityIndex]].containedAirportIDs;
      for(int fromIndex = 0; fromIndex < cityAirports.size(); ++fromIndex) {
         vector<int> outgoingAirports = airports[cityAirports[fromIndex]].outgoingIDs;
         int fromID = cityAirports[fromIndex];
         airMap[cityIndex].resize(cityNames.size());
//cout << "fromID: " << fromID << endl;

         for(int toIndex = 0; toIndex < outgoingAirports.size(); ++toIndex) {
            int destCityIndex = airports[outgoingAirports[toIndex]].cityID;
            int toID = outgoingAirports[toIndex];
//cout << "toID: " << toID << endl;
//cout << "destCityIndex: " << destCityIndex << endl;
//cout << "rtIndex: " << rtIndex << endl;
//cout << "CITYNAME: " << cityNames[destCityIndex] << endl;
            routes[rtIndex].from = cityIndex;
            routes[rtIndex].to = destCityIndex;
            routes[rtIndex].distance = get_distance(airports[fromID].lat,
                      airports[fromID].lon, airports[toID].lat, airports[toID].lon);
            cityMap[cityIndex][destCityIndex] = routes[rtIndex].distance;
            airMap[cityIndex][destCityIndex].push_back(rtIndex);
            ++rtIndex;
         }
      }
   }

  /* for(int i = 0; i < 5; i++) {
      cout << "Route: " << i << " from=" << routes[i].from << " to=" << routes[i].to << " dist=" << routes[i].distance << endl;
   }*/
}

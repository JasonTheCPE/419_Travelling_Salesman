
#include <iostream>
#include <string>
#include <math.h>       /* sin */
#include "AirParse.h"
#include "airport.h"
#include "tsp.h"

#define PI 3.14159265
#define Deg2Rad(a) (a * 3.14159265/180.0)

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
   double tempLat, tempLon;
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

                  if(airports.find(tempInt) == airports.end() || conv.fail()) {
                     // Airport has no routes from in the route table, skip rest of line
                     skip = true;
                  } else {
                     // Airport has routes from, continue gathering info
                  }
                  break;
               case CITYNAME_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  cityName = line.substr(prev, pos - prev);
                  break;
               case LAT_INDEX:
                  //cout << "LAT: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempLat;

                  if(conv.fail()) {
                     skip = true;  
                  }
                  break;
               case LON_INDEX:
                  //cout << "LON: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempLon;

                  if(conv.fail()) {
                     skip = true;
                  } else {
                     airports[tempInt].cityID = cityNames.size();
                     airports[tempInt].cityName = string(cityName);
                     if(cities.find(cityName) == cities.end()) {
                        // Add cities to the name list only once
                        cityNames.push_back(cityName);
                     }

                     cities[cityName].containedAirportIDs.push_back(tempInt); 
                     airports[tempInt].lat = tempLat;
                     airports[tempInt].lon = tempLon;
                  }
                  break;
               default:
                  break;
            }
            ++index;
         }
         prev = pos + 1;
      }
   }
#ifdef DEBUG
   cout << "Cities: " << cities.size() << endl;
#endif
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
   bool skip;

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
      skip = false;

      while ((pos = line.find_first_of(delimiters, prev)) != string::npos && !skip)
      {
         if (pos > prev) {
            switch(index) {
               case SOURCEALIAS_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  // Grab the alias for assignment in the later steps
                  tempStr = line.substr(prev, pos - prev);

                  if(tempStr.length() < 3) {
                     skip = true;
                  }
                  break;
               case SOURCEID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> sourceID;

                  if(conv.fail()) {
                     skip = true;
                  } else {
                     // Add alias to each airport
                     airports[sourceID].alias = tempStr;
                  }
                  break;
               case DESTID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> destID;
                  
                  if(conv.fail()) {
                     skip = true;
                     airports.erase(sourceID);
                  } else {
                     // Add outgoing route information to each airport
                     airports[sourceID].outgoingIDs.push_back(destID);
                     ++routeNum;
                  }
                  break;
               default:
                  break;
            }
            ++index;
         }
         prev = pos + 1;
      }
   }

#ifdef DEBUG
   cerr << "Airports: " << airports.size() << endl;
   cerr << "Routes: " << routeNum << endl;
#endif
}

double get_distance(double lat1_a, double lat2_a, double lon1_b, double lon2_b) {
    double R = 6367.4447;    // radius of the earth according to wolfram alpha and Siri (who used wolfram)
    double lat1 = Deg2Rad(lat1_a);
    double lat2 = Deg2Rad(lat2_a);
    double lon1 = Deg2Rad(lon1_b);
    double lon2 = Deg2Rad(lon2_b);

    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;

    double u = sin(dlat / 2);
    double v = sin(dlon / 2);

    double a = u*u + cos(lat1) * cos(lat2) * v*v;
    double c = 2.0 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

/*
 * Populates various vectors with information needed for the Floyd-Warshall
 * algorithm to run.
 */
void FillRouteVector(std::vector<route> &routes,
                std::vector<std::vector<std::vector<int> > > &airMap,
                std::vector<std::vector<double> > &cityMap,
                std::map<std::string, city> &cities,
                std::vector<std::string> &cityNames,
                std::map<int, airport> &airports) {

   int rtIndex = 0;

   for(int cityIndex = 0; cityIndex < cityNames.size(); ++cityIndex) {
      vector<int> cityAirports = cities[cityNames[cityIndex]].containedAirportIDs;

      for(int fromIndex = 0; fromIndex < cityAirports.size(); ++fromIndex) {
         vector<int> outgoingAirports = airports[cityAirports[fromIndex]].outgoingIDs;
         int fromID = cityAirports[fromIndex];
         airMap[cityIndex].resize(cityNames.size());

         for(int toIndex = 0; toIndex < outgoingAirports.size(); ++toIndex) {
            int destCityIndex = airports[outgoingAirports[toIndex]].cityID;
            int toID = outgoingAirports[toIndex];

            double fromLat = airports[fromID].lat;
            double fromLon = airports[fromID].lon;
            double toLat = airports[toID].lat;
            double toLon = airports[toID].lon;
            double dist;

            dist = get_distance(fromLat, fromLon, toLat, toLon);
            
            routes[rtIndex].from = cityIndex;
            routes[rtIndex].to = destCityIndex;
            routes[rtIndex].fromID = fromID;
            routes[rtIndex].toID = toID;
            routes[rtIndex].distance = dist;

            if(airMap[cityIndex][destCityIndex].size() > 0) {
               //update if lower distance found
               if(dist < cityMap[cityIndex][destCityIndex]) {
                  airMap[cityIndex][destCityIndex][0] = rtIndex;
                  cityMap[cityIndex][destCityIndex] = dist;
               }
            } else {
               airMap[cityIndex][destCityIndex].push_back(rtIndex);
               cityMap[cityIndex][destCityIndex] = dist;
            }
            ++rtIndex;
         }
      }
   }
}


#include <iostream>
#include <string>
#include "AirParse.h"

using namespace std;

void ParseAirports(const char* filename, airport_base *ab, route_base *rt) {
   ifstream inFile(filename);
   stringstream ss, conv;
   string line;
   string delimiters = ",\"";
   int numAirports = 0;
   int index;
   size_t prev, pos;

   int tempInt;
   double tempDbl;

/*
 //I wish we could use thee regular expression, sadly tis not to be *sadness*
   regex regEx("^(\\d{1,}),\"([a-zA-Z ]{1,})\".*?\"([a-zA-Z]{3})\",.*?,([0-9\\-\\.]{3,}),([0-9\\-\\.]{3,}),");
*/

   if(!inFile) {
      cerr << "Problem reading from " << filename << endl;
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
               case ID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> tempInt;
                  if(rt->idToIndexMap.find(tempInt) == rt->idToIndexMap.end()) {
                     // Airport has no routes from in the route table, skip rest of line
                     prev = line.length();
                  } else {
                     ab->ids.push_back(tempInt);
                  }
                  break;
               case CITYNAME_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  ab->cities.push_back(line.substr(prev, pos - prev));
                  break;
               case ALIAS_INDEX:
                  //cout << "ALIAS: " << line.substr(prev, pos - prev) << endl;
                  ab->alias.push_back(line.substr(prev, pos - prev));
                  break;
               case LAT_INDEX:
                  //cout << "LAT: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempDbl;
                  ab->lats.push_back(tempDbl);
                  break;
               case LON_INDEX:
                  //cout << "LON: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempDbl;
                  ab->longs.push_back(tempDbl);
                  break;
               default:
                  break;
            }
            ++index;
         }
         prev = pos + 1;
      }
      ++numAirports;
   }
   
   ab->numAirports = numAirports;
   cout << "Airports: " << numAirports << endl;
}


void ParseRoutes(const char* filename, route_base::route_base *rt) {
   ifstream inFile(filename);
   stringstream ss, conv;
   string line;
   string delimiters = ",";
   int numRoutes = 0;
   int index;
   size_t prev, pos;

   int tempInt;

   if(!inFile) {
      cerr << "Problem reading from " << filename << endl;
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
                  rt->idToIndexMap.insert(make_pair<int, int>(rt->sourceID, rt->sourceAlias.size()));
                  rt->sourceAlias.push_back(line.substr(prev, pos - prev));
                  break;
               case SOURCEID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> tempInt;
                  rt->sourceID.push_back(tempInt);
                  break;
               case DESTALIAS_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  rt->destAlias.push_back(line.substr(prev, pos - prev));
                  break;
               case DESTID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> tempInt;
                  rt->destID.push_back(tempInt);
                  break;
               default:
                  break;
            }
            ++index;
         }
         prev = pos + 1;
      }
      ++numRoutes;
   }
   
   rt->numRoutes = numRoutes;
   cout << "Routes: " << numRoutes << endl;
}


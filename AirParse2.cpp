
#include <iostream>
#include <string>
#include "AirParse2.h"
#include "airprt.h"

using namespace std;

void GetAirportInfo(const char* airportsFilename, std::map<int,airprt::airprt> *ap) {
   ifstream inFile(airportsFilename);
   stringstream ss, conv;
   string line;
   string delimiters = ",\"";
   int numAirports = 0;
   int index;
   size_t prev, pos;

   int tempInt;
   double tempDbl;

   if(!inFile) {
      cerr << "Problem reading from " << airportsFilename << endl;
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

                  if((*ap).find(tempInt) == (*ap).end()) {
                     // Airport has no routes from in the route table, skip rest of line
                     prev = line.length();
                  } else {
                     // Airport has routes from, continue gathering info
                     ++numAirports;
                  }
                  break;
               case CITYNAME_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  (*ap)[tempInt].city = line.substr(prev, pos - prev);
                  break;
               case ALIAS_INDEX:
                  //cout << "ALIAS: " << line.substr(prev, pos - prev) << endl;
                  (*ap)[tempInt].alias = line.substr(prev, pos - prev);
                  break;
               case LAT_INDEX:
                  //cout << "LAT: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempDbl;
                  (*ap)[tempInt].lat = tempDbl;
                  break;
               case LON_INDEX:
                  //cout << "LON: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv.str(line.substr(prev, pos - prev));
                  conv >> tempDbl;
                  (*ap)[tempInt].lat = tempDbl;
                  break;
               default:
                  break;
            }
            ++index;
         }
         prev = pos + 1;
      }
   }
   
   //numAirports = numAirports;
   cout << "Airports: " << numAirports << endl;
}


void PopulateRoutes(const char* routeFilename, std::map<int,airprt::airprt> *ap) {
   ifstream inFile(routeFilename);
   stringstream ss, conv;
   string line;
   string delimiters = ",";
   int numRoutes = 0;
   int index;
   size_t prev, pos;

   int sourceID, destID;
   string tempStr;

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
                  (*ap)[sourceID].alias = tempStr;
                  break;
               case DESTALIAS_INDEX:
                  //cout << "CITYNAME: " << line.substr(prev, pos - prev) << endl;
                  // Don't care about pulling this data from this file
                  break;
               case DESTID_INDEX:
                  //cout << "ID: " << line.substr(prev, pos - prev) << endl;
                  conv.clear();
                  conv.str("");
                  conv << line.substr(prev, pos - prev);
                  conv >> destID;
                  // Add outgoing route information to each airport
                  (*ap)[sourceID].outgoingIDs.push_back(destID);
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
   
   //rt->numRoutes = numRoutes;
   cout << "Routes: " << numRoutes << endl;
}


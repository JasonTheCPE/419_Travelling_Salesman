
#include <iostream>
#include <string>
#include "AirParse.h"

using namespace std;

int ParseAirports(const char* filename, airport_base *ab) {
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
                  ab->ids.push_back(tempInt);
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
            //cout << line.substr(prev, pos-prev) << endl;
            ++index;
            //wordVector.push_back(line.substr(prev, pos-prev));
         }
         prev = pos + 1;
      }

      ++numAirports;

      if(numAirports > 5) {
         break;
      }
   }
   
   ab->numAirports = numAirports;
   cout << "Airports: " << numAirports << endl;
   return 0;
}

/*
int ParseRoutes(string filename){//, route_table *rt) {
   return 0;
}
*/

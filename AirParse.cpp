
#include <stdio.h>
#include <fstream.h>
#include <iostream.h>
#include <string.h>
#include "AirParse.h"

using namespace std;

#define ID_INDEX = 0
#define CITYNAME_INDEX = 2
#define ALIAS = 4
#define LAT = 6
#define LON = 7

void ParseAirports(string filename, airport_base *ab) {
   ifstream iFile(filename);
   char *line;
   char *separators = ",";
   char *tokens;
   int index;

   while(getline(iFile, line)) {
      tokens = (line, separators);
      index = 0;
      while(index tokens != NULL) {
         if() {
            
         }
      }
   }
   
}


void ParseRoutes(string filename, route_table *rt) {

}

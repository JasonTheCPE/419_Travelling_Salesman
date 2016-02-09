
#include "AirParse.h"

using namespace std;

int ParseAirports(char *filename, airport_base *ab) {
   ifstream inFile(filename);
   stringstream ss;
   string line;
   string delimiters = ",\"";
   int numAirports = 0;
   int index;
   size_t prev, pos;

/* //I wish we could use thee regular expression, sadly tis not to be *sadness*
   regex regEx("^(\\d{1,}),\"([a-zA-Z ]{1,})\".*?\"([a-zA-Z]{3})\",.*?,([0-9\\-\\.]{3,}),([0-9\\-\\.]{3,}),");
*/

   if(!inFile) {
      cerr << "Problem reading from " << filename << endl;
   }
   
   //string str = "5768,\"San Luis County Regional Airport\",\"San Luis Obispo\",\"United States\",\"SBP\",\"KSBP\",35.2368,-120.642,212,-8,\"A\",\"America/Los_Angeles\"";
   
   //while(getline(inFile, line)) {
      //istringstress(line);
      
   while(getline(inFile, line)) 
   {
      ss.str(line);
      prev = index = 0;

      while ((pos = line.find_first_of(delimiters, prev)) != string::npos)
      {
         if (pos > prev) {
            cout << line.substr(prev, pos-prev) << endl;
            //wordVector.push_back(line.substr(prev, pos-prev));
         }
         prev = pos + 1;
      }
      if (prev < line.length()) {
         //wordVector.push_back(line.substr(prev, std::string::npos));
         cout << line.substr(prev, string::npos) << endl;
      }
      ss.flush();
      ++numAirports;

      if(numAirports > 5) {
         break;
      }
   }
   
   
   cout << "Airports: " << numAirports << endl;
   return 0;
}

/*
int ParseRoutes(string filename){//, route_table *rt) {
   return 0;
}
*/

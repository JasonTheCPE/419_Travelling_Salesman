#ifndef _AIRPORT_BASE_
#define _AIRPORT_BASE_

typedef struct
 {
    int numAirports;
    std::vector<int> ids;
    std::vector<string> cities;
    std::vector<string> names;      // can throw out later
    std::vector<string> alias;
    std::vector<double> lats;
    std::vector<double> longs;
 } airport_base;

#endif
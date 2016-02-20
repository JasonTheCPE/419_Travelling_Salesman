#ifndef _TSP_
#define _TSP_

#include "airport.h"

void createFloydTable(int numCities, std::vector< std::vector<double> > &cityMap, 
                        std::vector< std::vector< std::vector<int> > > &routeMap);
std::vector<int> createRoute(int numCities, int startIdx, std::vector< std::vector<double> > &cityMap, 
                            std::vector<std::vector<std::vector<int> > > &routeMap);
void printToCSV(const char* filename, std::vector<int> &path, 
                std::vector<route> &routeList, std::map<int, airport> airports,
                std::vector<std::string> &cityNames);

#endif

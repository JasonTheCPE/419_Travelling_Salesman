#ifndef _TSP_
#define _TSP_

#include "airport.h"

double get_distance(double lat1_a, double lat2_a, double lon1_b, double lon2_b);
void createFloydTable(int numCities, double**cityMap, std::vector<std::vector<std::vector<int> > > &routeMap);
std::vector<int> createRoute(int numCities, int startIdx, std::vector< std::vector<double> > &cityMap, 
                            std::vector<std::vector<std::vector<int> > > &routeMap);
void printToCSV(const char* filename, std::vector<int> &path, 
                std::vector<route> &routeList, std::map<int, airport> airports);

#endif
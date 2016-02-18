#ifndef _TSP_
#define _TSP_

double get_distance(double lat1_a, double lat2_a, double lon1_b, double lon2_b);
void createFloydTable(int numCities, double**cityMap, std::vector<std::vector<std::vector<int> > > routeMap);
std::vector<int> createRoute(int numCities, std::vector<bool> visitedTable, 
                            double**cityMap, std::vector<std::vector<std::vector<int> > > routeMap);

double get_distance(int a_Id, int b_Id);
#endif
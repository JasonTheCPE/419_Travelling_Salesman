#include <math.h>       /* sin */
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <stdio.h>
#include <iostream>
#include <limits>

#include "tsp.h"

using namespace std;

//floyd algorithm, get any two points's minimum distance
void createFloydTable(int numCities, std::vector< std::vector<double> > &cityMap, 
                        std::vector< std::vector< std::vector<int> > > &routeMap) {

    #pragma omp parallel for                    // would need to send in routemap into xeonphi
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (routeMap[i][j].size() == 1)
                routeMap[i][j].push_back(j);
        }
    }

    for (int k = 0; k < numCities; k++) {
#ifdef DEBUG
        cout << "Progress: " << 100.0 *  k / numCities << "%  \r";
        fflush(stdout);
#endif
        std::vector<double> &city_ks = cityMap[k];
        std::vector< std::vector<int> > &routes_ks = routeMap[k];

        #pragma omp parallel for
        for (int i = 0; i < numCities; i++) {
            double fromDist = cityMap[i][k];
            std::vector<double> &city_is = cityMap[i];
            std::vector< std::vector<int> > &routes_is = routeMap[i];
            // i, k, fromDist, toDists, city_is, routes_ks, routes_is
            #pragma omp parallel for
            for (int j = 0; j < numCities; j++) {
                if (i != j && i != k && j != k) {
                    double newLength = fromDist + city_ks[j];

                    if (newLength < city_is[j]) {
                        city_is[j] = newLength;

                        // create the new vector
                        routes_is[j].clear();
                        routes_is[j].reserve(routes_is[k].size() + routes_ks[j].size()); // preallocate memory
                        routes_is[j].insert(routes_is[j].end(), routes_is[k].begin(), routes_is[k].end());
                        routes_is[j].insert(routes_is[j].end(), routes_ks[j].begin(), routes_ks[j].end());
                    }
                }
            }
        }
    }
}

void updateRoutes(bool *visitedTable, std::vector<int> &path, std::vector <int> routes, int numCities) {
    int size = routes.size();

    for (int j = 0; j < size; j += 2)
    {
        int toAdd = routes[j];
        path.push_back(toAdd);
        visitedTable[routes[j + 1]] = true;
    }
}

// very naive TSP
std::vector<int> createRoute(int numCities, int startIdx, std::vector< std::vector<double> > &cityMap, 
                            std::vector< std::vector< std::vector<int> > > &routeMap) {
    std::vector<int> path;
    path.reserve(numCities*2);

    bool *visitedTable = (bool *)calloc(numCities, sizeof(bool));
    int lastCity = startIdx;
    double *cities = &cityMap[startIdx][0];
    double MAX = numeric_limits<double>::max();

#pragma offload target(mic) in(cities: length(numCities)) inout(visitedTable: length(numCities))
    {
        for (int i = 0; i < numCities; ++i)
            visitedTable[i] = cities[i] == MAX;
    }

    for (int i = 0; i < numCities; ++i)
    {
        if (!visitedTable[i])
        {
            updateRoutes(visitedTable, path, routeMap[lastCity][i], numCities);
            lastCity = i;
        }
    }

    if (lastCity != startIdx) {
        updateRoutes(visitedTable, path, routeMap[lastCity][startIdx], numCities);
    }

    free(visitedTable);

    return path;
}

void printToCSV(const char* filename, std::vector<int> &path, 
                std::vector<route> &routeList, std::map<int, airport> airports,
                std::vector<std::string> &cityNames) {
    ofstream Out_File(filename);

    // headings for file
    Out_File << "City,Airport Code,Trip Distance (km),Total Distance (km)" << endl;

    // do initial location
    route curRoute = routeList[path[0]];
    Out_File << cityNames[curRoute.from] << "," 
             << airports[curRoute.fromID].alias << "," 
             << 0 << "," << 0 << endl;

    // fill table
    double pathCost = 0;
    for (int i = 0; i < path.size(); ++i) {
        curRoute = routeList[path[i]];

        // get the current total cost after flight
        pathCost += curRoute.distance;

        Out_File << cityNames[curRoute.to] << "," 
                 << airports[curRoute.toID].alias << "," 
                 << curRoute.distance << "," 
                 << pathCost << endl;   
    }

    // close the file
    Out_File.close();
}

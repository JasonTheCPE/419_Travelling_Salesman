#include <math.h>       /* sin */
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <stdio.h>
#include <iostream>
#include <limits>

#include "tsp.h"

#define PI 3.14159265
#define Deg2Rad(a) (a * 3.14159265/180.0)

using namespace std;

double get_distance(double lat1_a, double lat2_a, double lon1_b, double lon2_b) {
    double R = 6367.4447;    // radius of the earth according to wolfram alpha and Siri (who used wolfram)
    double lat1 = Deg2Rad(lat1_a);
    double lat2 = Deg2Rad(lat2_a);
    double lon1 = Deg2Rad(lon1_b);
    double lon2 = Deg2Rad(lon2_b);

    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;

    double u = sin(dlat/2);
    double v = sin(dlon/2);

    double a = u*u + cos(lat1) * cos(lat2) * v*v;
    double c = 2.0 * atan2(sqrt(a), sqrt(1-a)) ;
    double d = R * c;

    return d;
}

//floyd algorithm, get any two points's minimum distance
void createFloydTable(int numCities, std::vector< std::vector<double> > &cityMap, 
                        std::vector< std::vector< std::vector<int> > > &routeMap) {

    //#pragma omp parallel for      // would need to send in routemap into xeonphi
    for (int i = 0; i < numCities; ++i) {
        //#pragma omp simd
        for (int j = 0; j < numCities; ++j) {
            if (routeMap[i][j].size() > 1) {
                cerr << "Error: route too long " << routeMap[i][j].size() << endl;
            }
            if (routeMap[i][j].size() == 1)
                routeMap[i][j].push_back(j);
        }
    }

    for (int k = 0; k < numCities; k++) {
        cout << "Progress: " << 100.0 *  k / numCities << "%  \r";
        fflush(stdout);
        for (int i = 0; i < numCities; i++) {
            //#pragma omp parallel for // need to send in routeMap and cityMap to xeonphi
            for (int j = 0; j < numCities; j++) {
                if (i != j && i != k && j != k) {
                    double newLength = cityMap[i][k]+cityMap[k][j];

                    if (newLength < cityMap[i][j]) {
                        cityMap[i][j] = newLength;

                        // create the new vector
                        routeMap[i][j].clear();
                        routeMap[i][j].reserve( routeMap[i][k].size() + routeMap[k][j].size() ); // preallocate memory
                        routeMap[i][j].insert( routeMap[i][j].end(), routeMap[i][k].begin(), routeMap[i][k].end() );
                        routeMap[i][j].insert( routeMap[i][j].end(), routeMap[k][j].begin(), routeMap[k][j].end() );
                    }
                }
            }
        }
    }

	ofstream Out_File("baddata.txt");
	for (int i = 0; i < numCities; ++i) {
		for (int j = 0; j < numCities; ++j) {
			for (int k = 0; k < routeMap[i][j].size(); k += 2) {
				bool printStuff = false;

				if (routeMap[i][j][k] > 61498) {
					Out_File << "route: " << routeMap[i][j][k] << " ";
					printStuff = true;
				}

				if (routeMap[i][j][k+1] > numCities) {
					Out_File << "city: " << routeMap[i][j][k + 1] << " ";
					printStuff = true;
				}

				if (printStuff) {
					Out_File << "at " << i << ", " << j << ", " << k << endl;
				}
			}
		}
	}
}

// very naive TSP
std::vector<int> createRoute(int numCities, int startIdx, std::vector< std::vector<double> > &cityMap, 
                            std::vector< std::vector< std::vector<int> > > &routeMap) {
    std::vector<int> path;
    path.reserve(numCities*2);

    bool *visitedTable = (bool *)calloc(numCities, sizeof(bool));
    int lastCity = startIdx;

    // #pragma omp parallel for simd
    for (int i = 0; i < numCities; ++i)
        if (cityMap[startIdx][i] == numeric_limits<double>::max())
            visitedTable[i] = true;

    for (int i = 0; i < numCities; ++i)
    {
        if (!visitedTable[i])
        {
            for (int j = 0; j < routeMap[lastCity][i].size(); j += 2)
            {
                int toAdd = routeMap[lastCity][i][j];
                path.push_back(toAdd);
                visitedTable[routeMap[lastCity][i][j + 1]] = true;
            }
            lastCity = i;
        }
    }

    if (lastCity != startIdx) {
        for (int j = 0; j < routeMap[lastCity][startIdx].size(); j += 2)
        {
            int toAdd = routeMap[lastCity][startIdx][j];
            path.push_back(toAdd);
            visitedTable[routeMap[lastCity][startIdx][j + 1]] = true;
        }
        lastCity = startIdx;
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

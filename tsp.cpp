#include <math.h>       /* sin */
#include <vector>
#include <fstream>

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
void createFloydTable(int numCities, double**cityMap, std::vector< std::vector< std::vector<int> > > &routeMap) {
    for (int k = 0; k < numCities; k++) {
        for (int i = 0; i < numCities; i++) {
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
}

// very naive TSP
std::vector<int> createRoute(int numCities, int startIdx, double**cityMap, 
                            std::vector< std::vector< std::vector<int> > > &routeMap) {
    std::vector<int> path;
    path.reserve(numCities*2);
    path.push_back(startIdx);

    bool *visitedTable = calloc(numCities, sizeof(bool));
    int start;

    for (int i = 0; i < numCities; ++i)
    {
        if (!visitedTable[i])
        {
            start = path.back();
            for (int j = 0; j < routeMap[start][i].size(); ++j)
            {
                int toAdd = routeMap[start][i][j];
                path.push_back(toAdd);
                visitedTable[toAdd] = true;
            }
        }
    }

    start = path.back();
    if (start != 0) {
        for (int j = 0; j < routeMap[start][0].size(); ++j)
        {
            int toAdd = routeMap[start][0][j];
            path.push_back(toAdd);
            visitedTable[toAdd] = true;
        }
    }

    free(visitedTable);

    return path;
}

void printToCSV(const char* filename, std::vector<int> &path, 
                std::vector<route> &routeList, std::map<int, airport> airports) {
    ofstream Out_File(fileName);

    // headings for file
    Out_File << "City;Airport Code;Trip Distance (km);Total Distance (km)" << endl;

    // do initial location
    route curRoute = routeList[0];
    airport curAirport = airport[curRoute.from];
    Out_File << curAirport.cityName << ";" 
             << curAirport.alias << ";" 
             << 0 << ";" << 0 << endl;

    // fill table
    for (int i = 0, double pathCost = 0; i < path.size(); ++i) {
        curRoute = routeList[i];
        curAirport = airport[curRoute.to];

        // get the current total cost after flight
        pathCost += curRoute.distance;

        Out_File << curAirport.cityName << ";" 
                 << curAirport.alias << ";" 
                 << curRoute.distance << ";" 
                 << pathCost << endl;   
    }

    // close the file
    Out_File.close();
}
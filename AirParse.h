
#ifndef _AIRPARSE_
#define _AIRPARSE_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <utility>
#include <iterator>
#include "airport.h"

#define ID_INDEX 0
#define CITYNAME_INDEX 2
#define ALIAS_INDEX 4
#define LAT_INDEX 6
#define LON_INDEX 7

#define SOURCEALIAS_INDEX 2
#define SOURCEID_INDEX 3
#define DESTALIAS_INDEX 4
#define DESTID_INDEX 5

void GetAllInfo(const char* routeFilename, const char* airportsFilename,
                std::map<std::string, city> *cities,
                std::vector<std::string> *cityNames,
                std::map<int, airport> *airports, int *routeNum);

void GetRouteInfo(const char* routeFilename,
                  std::map<int, airport> *airports, int *routeNum);

void GetCityAirportsInfo(const char* airportsFilename,
                         std::map<std::string, city> *cities,
                         std::vector<std::string> *cityNames,
                         std::map<int, airport> *airports);

void FillRouteVector(std::vector<route> &routes,
                std::vector<std::vector<std::vector<int> > > &airMap,
                std::vector<std::vector<double> > &cityMap,
                std::map<std::string, city> &cities,
                std::vector<std::string> &cityNames,
                std::map<int, airport> &airports);
#endif

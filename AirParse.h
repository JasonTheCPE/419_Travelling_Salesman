
#ifndef _AIRPARSE_
#define _AIRPARSE_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "airport.h"

#define ID_INDEX 0
#define CITYNAME_INDEX 2
#define ALIAS_INDEX 4
#define LAT_INDEX 6
#define LON_INDEX 7

int ParseAirports(const char* filename, airport_base::airport_base *ab);
//int ParseRoutes(std::string filename, route_table *rt);

#endif

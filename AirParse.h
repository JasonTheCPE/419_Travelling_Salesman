
#ifndef _AIRPARSE_
#define _AIRPARSE_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "airport.h"
#include "route.h"

#define ID_INDEX 0
#define CITYNAME_INDEX 2
#define ALIAS_INDEX 4
#define LAT_INDEX 6
#define LON_INDEX 7

#define SOURCEALIAS_INDEX 2
#define SOURCEID_INDEX 3
#define DESTALIAS_INDEX 4
#define DESTID_INDEX 5

void ParseAirports(const char* filename, airport_base::airport_base *ab);
void ParseRoutes(const char* filename, route_base::route_base *rt);

#endif

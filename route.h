#ifndef _ROUTE_BASE_
#define _ROUTE_BASE_

#include <vector>
#include <string>
#include <map>

using namespace std;

typedef struct route_base
 {
    int numRoutes;
    std::vector<int> sourceID;
    std::vector<int> destID;

    std::vector<string> sourceAlias;
    std::vector<string> destAlias;

    std::map<int, int> idToIndexMap;

 } route_base;

#endif

#ifndef _ROUTE_BASE_
#define _ROUTE_BASE_

typedef struct
 {
    int numRoutes;
    std::vector<int> sourceID;
    std::vector<int> destID;

    std::vector<string> sourceAlias;
    std::vector<string> destAlias;

 } route_base;

#endif
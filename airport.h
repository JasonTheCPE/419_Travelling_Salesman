#ifndef _AIRPORT_INFO_
#define _AIRPORT_INFO_

#include <vector>
#include <string>

typedef struct airport
{
   std::string cityName;
   int cityID;
   std::string alias;
   double lat;
   double lon;
   std::vector<int> outgoingIDs;
}airport;

typedef struct city
{
   std::vector<int> containedAirportIDs;
   //int stringVectorID;
   //std::vector<int> nextVisitedAirportID;
   //std::vector<std::string> nextVisitedCity;
   //int visitedCount;
}city;

typedef struct route
{
   int from;
   int to;
   int fromID;
   int toID;
   double distance;
}route;

#endif

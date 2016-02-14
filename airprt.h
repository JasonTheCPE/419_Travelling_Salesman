#ifndef _AIRPORT_BASE_
#define _AIRPORT_BASE_

#include <vector>
#include <string>

using namespace std;

typedef struct airprt
{
   string city;
   string alias;
   double lat;
   double lon;
   vector<int> outgoingIDs;
} airprt;

#endif

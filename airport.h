#ifndef _AIRPORT_BASE_
#define _AIRPORT_BASE_

#include <vector>
#include <string>

class airport_base
{
public:
    airport_base();
    airport_base(int size);
    //~airport_base();
    
    int numAirports;
    std::vector<int> ids;        //Ex: 5768
    std::vector<std::string> cities;  //Ex: San Luis Obispo
    //std::vector<string> names;   //Ex: ???
    std::vector<std::string> alias;   //Ex: SBP
    std::vector<double> lats;    //Ex: 35.2368
    std::vector<double> longs;   //Ex: -120.624

    double get_distance(int a_Id, int b_Id);
};

#endif

#ifndef _AIRPORT_BASE_
#define _AIRPORT_BASE_

class airport_base
{
public:
    airport_base();
    airport_base::airport_base(int size)
    ~airport_base();
    
    int numAirports;
    std::vector<int> ids;
    std::vector<string> cities;
    std::vector<string> names;      // can throw out later
    std::vector<string> alias;
    std::vector<double> lats;
    std::vector<double> longs;

    double get_distance(int a_Id, int b_Id);
};

#endif
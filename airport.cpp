#include <airport.h>

using namespace std;

airport_base::airport_base() {
    numAirports = 0;
}

airport_base::airport_base(int size) {
    numAirports = size;
    ids.resize(size);
    cities.resize(size);
    names.resize(size);
    alias.resize(size);
    lats.resize(size);
    longs.resize(size);
}

double airport_base::get_distance(int a_Id, int b_Id) {
    return 0;
}
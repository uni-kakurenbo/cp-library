#include<bits/stdc++.h>
using namespace std;
#include "template/standard.hpp"
#include "data_structure/bit_vector.hpp"

signed main() {
    lib::bit_vector data(100);
    data.build();
    debug(data.size());
    debug(data);

    REP(i, 105) debug(data.select0(i));
}

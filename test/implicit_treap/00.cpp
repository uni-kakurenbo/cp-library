// https://judge.yosupo.jp/problem/range_affine_range_sum

#include <atcoder/modint>
#include "template.hpp"

#include "data_structure/range_operations.hpp"
using namespace Lib;

#include <vector>

signed main() {
    Lib::RangeUpdateRangeMin<int,int> data;

    debug(data);
    {
        data.insert(0, 10);
        data.insert(1, 5);
        data.insert(2, 75);
        data.insert(3);
        data.insert(4, 13);
    }
    debug(data);

    {
        data.apply(0, 1, 0);
        data.apply(1, 2, 10);
        data.apply(2, 3, 15);
        data.apply(3, 4, 20);
        data.apply(4, 5, 30);
    }
    debug(data);

    data.apply(1, 3, 25);
    debug(data);

    data.reverse(0, 5);
    debug(data);

    data.reverse(1, 4);
    debug(data);

    debug(data.prod(0, 1));
    debug(data.prod(2, 5));
    debug(data.prod(2, 3));
}

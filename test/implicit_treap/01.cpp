// https://judge.yosupo.jp/problem/range_affine_range_sum

#include <atcoder/modint>
#include "template.hpp"

#include "data_structure/range_operation/range_add_range_sum.hpp"
using namespace Lib;

#include <vector>

signed main() {
    Lib::RangeAddRangeSum<int,int> data;

    {
        data.insert(0, 1);
        data.insert(1, 2);
        data.insert(2, 3);
        data.insert(3, 4);
        data.insert(4, 5);
    }
    debug(data);

    data.rotate(0, 1, 3);
    debug(data);

    {
        debug(data.get(0));
        debug(data.get(1));
        debug(data.get(2));
        debug(data.get(3));
    }
    debug(data);

    data.reverse();
    debug(data);

    data.clear();
    debug(data, data.size());

    data.resize(12, 19);
    debug(data);

    data.fill(12);
    debug(data);

    data.resize(8, 5);
    debug(data);

    data.resize(15, 15);
    debug(data);

    data.push_back(135);
    debug(data);

    data.insert(5, 30, 7);
    debug(data);

    data.fill(2, 9, 98);
    debug(data);

    data.assign(20, 10);

    {
        std::vector<int> vec = { 0, 1, 2, 3, 4, 5 };
        data.insert(12, ALL(vec));
        data.insert(0, ALL(vec));
        data.insert(data.size(), ALL(vec));
        debug(data);
    }

    data.add(12);
    debug(data);

    data.add(3, 8, 10);
    debug(data);

    data.pop_back(3);
    debug(data);

    data.pop_front(5);
    debug(data);

    data.push_front(2, 5);
    debug(data);

    data.push_back(3, 4);
    debug(data);
    debug(data.front(), data.back());
}

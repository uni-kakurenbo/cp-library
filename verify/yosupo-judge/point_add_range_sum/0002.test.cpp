/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/vector.hpp"
#include "adaptor/io.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "action/range_add_range_sum.hpp"


signed main() {
    int n, q; std::cin >> n >> q;
    uni::vector<uni::ll> a(n); input >> a;

    uni::dynamic_sequence<uni::actions::range_add_range_sum<uni::ll>> data(a);
    // debug(data._root->rank);
    debug(data.size());
    // debug(data.dump_rich());

    REP(i, q) {
        int t; std::cin >> t;
        if(t == 0) {
            uni::ll p, x; std::cin >> p >> x;
            if(i % 2) data[p] += x;
            else data[p] *= x;
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r;
            print(data(l, r).fold());
        }
        debug(data.dump_rich());
        // if(i == 10) break;
    }

    return 0;
}

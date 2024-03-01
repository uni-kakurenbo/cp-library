/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/vector.hpp"
#include "adapter/io.hpp"
#include "data_structure/segment_tree.hpp"
#include "action/range_sum.hpp"


signed main() {
    int n, q; std::cin >> n >> q;
    lib::vector<lib::ll> a(n); input >> a;

    lib::segment_tree<lib::actions::range_sum<lib::ll>> data(a);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            lib::ll p, x; std::cin >> p >> x;
            data.set(p, data[p].val() + x);
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r;
            print(data.fold(l, r));
        }
    }

    return 0;
}

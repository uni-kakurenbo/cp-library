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
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "action/range_sum.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    uni::vector<uni::ll> a(n); input >> a;

    uni::fenwick_tree<uni::actions::range_sum<uni::ll>> data(a);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, x; std::cin >> p >> x;
            data[p] += x;
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r;
            print(data(l, r).fold());
        }
    }

    return 0;
}

/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "action/range_add_range_sum.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    uni::lazy_segment_tree<uni::actions::range_add_range_sum<uni::ll>> data(n);

    for(;q--;) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; --l;
            if(l + 1 == r) data[l] += x;
            else data(l, r) *= x;
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; --l;
            print(data(l, r).fold());
        }
    }
}

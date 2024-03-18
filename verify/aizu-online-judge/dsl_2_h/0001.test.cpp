/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_H"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "action/range_affine_range_minmax.hpp"

signed main() {
    int n, q; std::cin >> n >> q;

    lib::lazy_segment_tree<lib::actions::range_affine_range_minmax<lib::ll>> data(n, lib::spair<lib::ll>{ 0, 0 });

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; ++r;
            data(l, r) += { 1, x };
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; ++r;
            print(data(l, r).fold()->first);
        }
        // debug(data);
    }
}

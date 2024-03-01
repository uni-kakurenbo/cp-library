/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_I"


#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "action/range_set_range_sum.hpp"

signed main() {
    int n, q; input >> n >> q;

    lib::lazy_segment_tree<lib::actions::range_set_range_sum<lib::i64>> data(n);

    REP(q) {
        int t, l, r; input >> t >> l >> r; ++r;
        if(t == 0) {
            int x; input >> x;
            data(l, r) += x;
        }
        if(t == 1) {
            print(data(l, r).fold());
        }
    }
}

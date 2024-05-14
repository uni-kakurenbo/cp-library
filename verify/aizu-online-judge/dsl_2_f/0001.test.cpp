/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "action/range_set_range_min.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;

    uni::lazy_segment_tree<uni::actions::range_set_range_min<uni::i32>> data(n);

    REP(i, q) {
        uni::i32 t, l, r; input >> t >> l >> r; ++r;
        if(t == 0) {
            uni::i32 x; input >> x;
            if(l + 1 == r) data[l] = x;
            else data(l, r) *= x;
        }
        if(t == 1) {
            print(data(l, r).fold());
        }
    }
}

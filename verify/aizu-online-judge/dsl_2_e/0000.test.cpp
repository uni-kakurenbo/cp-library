/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_E"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "action/range_add.hpp"
#include "action/helpers.hpp"
#include "algebraic/addition.hpp"
#include "template/debug.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;

    uni::lazy_segment_tree<
        uni::actions::make_full_t<
            uni::actions::range_add<uni::ll>
        >
    > data(n);

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 l, r, x; input >> l >> r >> x; --l;
            data(l, r) *= x;
        }
        if(t == 1) {
            uni::i32 p; input >> p; --p;
            print(data[p]);
        }
        debug(data);
    }

    return 0;
}

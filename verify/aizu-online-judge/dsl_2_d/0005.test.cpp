/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_D"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/red_black_tree.hpp"
#include "action/range_set.hpp"
#include "action/helpers.hpp"
#include "algebraic/addition.hpp"
#include "template/debug.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;

    uni::dynamic_sequence<
        uni::actions::make_full_t<uni::i32>,
        uni::red_black_tree_context<uni::i32>
    > data(n, (1U << 31) - 1);

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 l, r, x; input >> l >> r >> x; ++r;
            data(l, r) = x;
        }
        if(t == 1) {
            uni::i32 p; input >> p;
            print(data[p]);
        }
        debug(data);
    }

    return 0;
}

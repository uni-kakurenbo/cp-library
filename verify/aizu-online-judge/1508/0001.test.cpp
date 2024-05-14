/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1508"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/red_black_tree.hpp"
#include "action/range_min.hpp"
#include "action/helpers.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::vector<uni::i32> a(n); input >> a;

    uni::dynamic_sequence<uni::actions::make_full_t<uni::actions::range_min<uni::i32>>, uni::red_black_tree_context<uni::i32>> data(a);
    debug(data.dump_rich());

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 l, r; input >> l >> r; ++r;
            data.rotate_right(l, r);
        }
        if(t == 1) {
            uni::i32 l, r; input >> l >> r; ++r;
            print(data(l, r).fold());
        }
        if(t == 2) {
            uni::i32 p, x; input >> p >> x;
            data[p] = x;
        }
        debug(data.dump_rich());
    }
}

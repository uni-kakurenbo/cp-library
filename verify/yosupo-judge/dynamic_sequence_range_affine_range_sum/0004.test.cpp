/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "numeric/modular/modint.hpp"
#include "adaptor/vector.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/red_black_tree.hpp"
#include "action/range_affine_range_sum.hpp"

using mint = uni::modint998244353;

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::vector<mint> a(n); input >> a;

    uni::dynamic_sequence<
        uni::actions::range_affine_range_sum<mint>,
        uni::red_black_tree_context<uni::i32>
    > data(a);

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 p, x; input >> p >> x;
            data.insert(p, mint{ x });
        }
        if(t == 1) {
            uni::i32 p; input >> p;
            data.erase(p);
        }
        if(t == 2) {
            uni::i32 l, r; input >> l >> r;
            data.reverse(l, r);
        }
        if(t == 3) {
            uni::i32 l, r, b, c; input >> l >> r >> b >> c;
            data(l, r) *= { b, c };
        }
        if(t == 4) {
            uni::i32 l, r; input >> l >> r;
            print(data(l, r).fold());
            // break;
        }

        debug(data.dump_rich());
    }
}

/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include <memory>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "numeric/modular/modint.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "action/range_composition.hpp"
#include "action/range_set_range_composition.hpp"
#include "algebraic/affine.hpp"


using mint = uni::modint998244353;

signed main() {
    uni::dynamic_sequence<uni::actions::range_composition<mint>> data;

    uni::i32 q; input >> q;
    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            mint a, b; input >> a >> b;
            data.push_back({ a, b });
        }
        if(t == 1) {
            data.pop_front();
        }
        if(t == 2) {
            mint x; input >> x;
            debug(data.fold());
            print(data.fold()(x));
        }
        debug(data);
    }
}

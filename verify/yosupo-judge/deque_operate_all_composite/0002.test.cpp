/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/deque_operate_all_composite"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include <memory>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "numeric/modular/modint.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/red_black_tree.hpp"
#include "action/range_composition.hpp"


using mint = uni::modint998244353;

signed main() {
    uni::dynamic_sequence<uni::actions::range_composition<mint>, uni::red_black_tree_context<uni::u32>> data;

    uni::i32 q; input >> q;
    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            mint a, b; input >> a >> b;
            data.push_front({ a, b });
        }
        if(t == 1) {
            mint a, b; input >> a >> b;
            data.push_back({ a, b });
        }
        if(t == 2) {
            data.pop_front();
        }
        if(t == 3) {
            data.pop_back();
        }
        if(t == 4) {
            mint x; input >> x;
            print(data.fold()(x));
        }
    }
}

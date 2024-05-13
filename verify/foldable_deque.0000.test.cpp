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
#include "data_structure/foldable_stack.hpp"
#include "algebraic/affine.hpp"
#include "algebraic/addition.hpp"


using mint = uni::modint998244353;

signed main() {
    uni::foldable_deque<uni::algebraic::affine<mint>> deque;

    uni::i32 q; input >> q;
    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            mint a, b; input >> a >> b;
            deque.emplace_front(a, b);
        }
        if(t == 1) {
            mint a, b; input >> a >> b;
            deque.emplace_back(a, b);
        }
        if(t == 2) {
            deque.pop_front();
        }
        if(t == 3) {
            deque.pop_back();
        }
        if(t == 4) {
            mint x; input >> x;
            print(deque.fold()(x));
        }
    }
}

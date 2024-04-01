/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/double_ended_priority_queue"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "data_structure/actable_dynamic_set.hpp"

signed main() {
    int n, q; std::cin >> n >> q;

    lib::actable_dynamic_set<lib::i32> set(({
        lib::vector<lib::i32> v(n); input >> v;
        v;
    }));

    REP(q) {
        int t; input >> t;
        if(t == 0) {
            lib::i32 x; input >> x;
            set.insert(x);
        }
        if(t == 1) print(set.pop_min());
        if(t == 2) print(set.pop_max());
    }
}

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
#include "data_structure/dynamic_set.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;

    uni::dynamic_set<uni::i32> set(({
        uni::vector<uni::i32> v(n); input >> v;
        v;
    }));

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 x; input >> x;
            set.insert(x);
        }
        if(t == 1) print(set.pop_min());
        if(t == 2) print(set.pop_max());
    }
}

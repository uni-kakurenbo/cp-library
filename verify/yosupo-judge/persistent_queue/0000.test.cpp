/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/persistent_queue"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include <memory>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/persistent_queue.hpp"

#include <memory_resource>

signed main() {
    uni::i32 q; input >> q;

    std::vector<uni::persistent_queue<uni::i32, std::bit_width(200'000U)>> queue(1);

    REP(q) {
        uni::i32 k; input >> k;
        if(k == 0) {
            uni::i32 t, x; input >> t >> x; ++t;
            queue.emplace_back(queue[t].clone().push(x));
        }
        if(k == 1) {
            uni::i32 t; input >> t; ++t;
            print(queue[t].front());
            queue.emplace_back(queue[t].clone().pop());
        }
    }
}

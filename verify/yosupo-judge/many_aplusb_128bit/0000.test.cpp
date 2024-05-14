/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb_128bit"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "numeric/int128.hpp"

signed main() {
    uni::i32 t; input >> t;
    REP(t) {
        uni::i128 a, b; input >> a >> b;
        print(a + b);
    }
}

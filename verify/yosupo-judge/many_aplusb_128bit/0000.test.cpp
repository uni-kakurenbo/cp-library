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
    int t; std::cin >> t;
    REP(t) {
        uni::i128 a, b; std::cin >> a >> b;
        print(a + b);
    }
}

/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/kth_root_integer"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "numeric/arithmetic.hpp"

signed main() {
    int t; std::cin >> t;
    REP(t) {
        lib::u64 a, k; std::cin >> a >> k;
        print(lib::kth_root_floor(a, k));
    }
}

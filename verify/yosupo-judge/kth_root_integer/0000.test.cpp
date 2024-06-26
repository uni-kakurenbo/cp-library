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
#include "adaptor/io.hpp"
#include "numeric/arithmetic.hpp"

signed main() {
    uni::i32 t; input >> t;
    REP(t) {
        uni::u64 a, k; input >> a >> k;
        print(uni::kth_root_floor(a, k));
    }
}

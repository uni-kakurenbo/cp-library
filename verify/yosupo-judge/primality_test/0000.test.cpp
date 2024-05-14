/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "numeric/fast_prime.hpp"

uni::i32 main() {
    uni::i32 q; input >> q;
    REP(q) {
        uni::i64 n; input >> n;
        print.yesno(uni::is_prime(n));
    }
}

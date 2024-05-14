/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "snippet/aliases.hpp"
#include "numeric/binomial_coefficient.hpp"

signed main() {
    uni::i32 t; input >> t;
    uni::i32 m; input >> m;

    uni::binomial_coefficient<uni::i32> binom(m, 10'000'000);

    REP(t) {
        uni::i32 n, k; input >> n >> k;
        print(binom.comb(n, k));
    }
}

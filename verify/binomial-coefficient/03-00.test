/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://atcoder.jp/contests/agc025/tasks/agc025_b"

#include <iostream>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "constants.hpp"
#include "numeric/modint.hpp"
#include "snippet/aliases.hpp"
#include "numeric/binomial_coefficient.hpp"

using mint = lib::modint998244353;

signed main() {
    lib::i64 n, a, b; std::cin >> n >> a >> b;
    lib::i64 k; std::cin >> k;

    lib::binomial_coefficient<lib::i64, lib::modint998244353> binom(n);

    mint ans = 0;

    FOR(x, n) {
        lib::i64 by = k - a * x;
        if(by % b != 0) continue;
        lib::i64 y = by / b;
        if(0 <= y and y <= n) ans += lib::MINT<1> * binom.comb(n, x) * binom.comb(n, y);
    }

    print(ans);
}

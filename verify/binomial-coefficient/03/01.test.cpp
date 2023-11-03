/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

#define PROBLEM "https://atcoder.jp/contests/agc025/tasks/agc025_b"

void solve();

signed main() {
    debug(__cplusplus);
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

#include "numeric/binomial_coefficient.hpp"

using mint = lib::modint998244353;

void solve() {
    i64 n, a, b; cin >> n >> a >> b;
    i64 k; cin >> k;

    lib::binomial_coefficient_prime_power_mod<i64,lib::modint998244353> binom(n);

    mint ans = 0;

    FOR(x, n) {
        i64 by = k - a * x;
        if(by % b != 0) continue;
        i64 y = by / b;
        if(0 <= y and y <= n) ans += MINT<1> * binom.comb(n, x) * binom.comb(n, y);
    }

    print(ans);
}

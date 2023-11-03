/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
// #include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

#define PROBLEM "https://judge.yosupo.jp/problem/lcm_convolution"

void solve();

signed main() {
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

#include "convolution/lcm.hpp"

using mint = lib::modint998244353;

void solve() {
    int n; cin >> n;
    valarray<mint> a(n), b(n); input >> a >> b;
    print(lib::lcm_convolution(ALL(a), ALL(b)));
}

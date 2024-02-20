/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template/standard.hpp"
/* #endregion */

void solve();

signed main() {
    debug(__cplusplus);
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        debug_("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

void solve() {
    int n; cin >> n;
    valarray<i64> a(n), b(n); input >> lib::views::zip(a, b);
    auto zipped = lib::views::zip(a, std::views::iota(2));
    debug(zipped.begin() == zipped.end());
    // static_assert(std::is_same_v<decltype(zipped.begin()),decltype(zipped.end())>);
    debug(a, b);
    // debug(lib::views::zip(std::views::iota(2), a, lib::prime_enumerator(10)));
}

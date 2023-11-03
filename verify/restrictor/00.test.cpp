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

#define PROBLEM "https://atcoder.jp/contests/abc293/tasks/abc293_f"

void solve();

signed main() {
    debug(__cplusplus);
    int $ = 1;
    std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

#include "numeric/boundary_seeker.hpp"

void solve() {
    i64 n; std::cin >> n;

    auto ok = [&](i64 b, i64 d) {
        auto s = lib::to_base_n_vector(n, b);
        if((long)s.size() != d) return false;
        debug(b, s);
        return std::count(ALL(s), 0) + std::count(ALL(s), 1) == (long)s.size();
    };

    i64 ans = 1;
    ans += (n >= 3);

    int d = lib::bit_width(lib::to_unsigned(n));

    constexpr i64 SUP = 2'000'000'000'000'000'000L;

    FOR(i, 3, d) {
        lib::boundary_seeker<i64> seeker(
            [&](auto x) { return lib::pow(lib::static_restrictor<i64,0,SUP>{x}, i-1) <= n; }
        );
        i64 b = seeker.bound(2);
        ans += ok(b, i);
    }

    print(ans);
}

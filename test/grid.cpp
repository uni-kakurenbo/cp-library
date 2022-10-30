/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>
using namespace std;

#include "template.hpp"
#include "output.hpp"

Output _print;
#define print _print
/* #endregion */

// https://atcoder.jp/contests/abc210/tasks/abc210_d

#include "template.hpp"

#include "utility/apply.hpp"
#include "grid.hpp"

signed main() {
    int h, w; cin >> h >> w;
    int c; cin >> c;

    GridBase<int> a(h, w); a.read();

    Min<ll> ans = INF64;

    auto solve = [&]() {
        GridBase<ll> dp(h+1, w+1, INF64);
        FOR(i, 1, h) FOR(j, 1, w) {
            if(chmin(dp(i,j), min(dp(i-1,j), dp(i,j-1)))) {
                dp(i,j) += c;
                ans <<= dp(i,j) + a(i-1,j-1);
            };
            chmin(dp(i,j), 1LL * a(i-1,j-1));
            debug(dp);
        }
    };

    solve();
    reverse(ALL(a));
    solve();
    REP(i, h) reverse(ALL(a[i]));
    solve();
    reverse(ALL(a));
    solve();

    print(ans);

    return 0;
}

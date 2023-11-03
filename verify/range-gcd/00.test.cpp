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

#define PROBLEM "https://atcoder.jp/contests/abc125/tasks/abc125_c"

#include "data_structure/disjoint_sparse_table.hpp"
#include "data_structure/range_action/range_gcd.hpp"

void solve() {
    int n; input >> n;
    valarray<i32> a(n); input >> a;

    lib::disjoint_sparse_table<lib::actions::range_gcd<i32>> data(a);

    maximum<i32> ans = 0;
    REP(i, n) {
        ans <<= (data(0, i).fold() + data(i + 1, n).fold()).val();
    }

    print(ans);
}

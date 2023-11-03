/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

#define PROBLEM "https://atcoder.jp/contests/abc294/tasks/abc294_c"

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

#include "template.hpp"
#include "data_structure/adapter/set.hpp"

void solve() {
    int n, m; cin >> n >> m;
    valarray<i64> a(n), b(m); input >> a >> b;
    lib::compressed comp(lib::views::concat(a, b));
    lib::multiset_adapter st(comp);
    auto f = [&](int v) { return st.count_or_under(comp.rank(v)); };
    print(a | std::views::transform(f));
    print(b | std::views::transform(f));
}

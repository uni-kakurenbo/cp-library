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

#define PROBLEM "https://atcoder.jp/contests/abc242/tasks/abc242_b"

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

#include "iterable/compressed.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/adapter/set.hpp"

void solve() {
    string s; cin >> s;
    ITRR(c, s) c -= 'a';
    lib::multiset_adapter st(ALL(s));
    string res;
    REP(i, s.size()) res += st.kth_smallest(i).value() + 'a';
    print(res);
}

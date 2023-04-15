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

#include "iterable/compression.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/adapter/set.hpp"

void solve() {
    string s; cin >> s;
    std::vector<int> cnt(26);
    ITR(v, s) cnt[v-'a']++;
    lib::multiset_adapter st(ALL(cnt));
    string res;
    REP(i, s.size()) res += st.kth_smallest(i).value() + 'a';
    print(res);
}

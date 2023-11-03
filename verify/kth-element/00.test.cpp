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

#define PROBLEM "https://yukicoder.me/problems/no/649"

#include "data_structure/kth_element.hpp"


void solve() {
    int q, k; input >> q >> k, --k;

    lib::kth_element<i64> data(k);

    REP(q) {
        int t; input >> t;
        if(t == 1) {
            i64 v; input >> v;
            data.push(v);
        }
        if(t == 2) {
            print(data.value_or(-1));
            data.pop();
        }
    }
}

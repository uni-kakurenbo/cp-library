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

#include "data_structure/lazy_segment_tree.hpp"
#include "data_structure/range_action/range_set_range_sum.hpp"

using mint = lib::modint998244353;

valarray<mint> ten, one;

void solve() {
    int n, q; cin >> n >> q;

    ten.resize(n + 1), one.resize(n + 1);
    ten[0] = 1, one[0] = 0;
    REP(i, n) {
        ten[i+1] = ten[i] * 10;
        one[i+1] = one[i] + ten[i];
    }

    lib::lazy_segment_tree<action<mint>> data(n, mint{1});

    valarray<monoid<mint>> a(n, mint{1});
    REP(q) {
        int l, r, d; cin >> l >> r >> d; --l;
        data(l, r) <<= d;
        print(data.fold_all()->first);
    }
}

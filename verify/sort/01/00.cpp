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
#include <boost/range/join.hpp>

void solve() {
    int n, m; cin >> n >> m;
    valarray<i64> a(n), b(m); input >> a >> b;
    auto rng = boost::join(a, b);
    lib::compression comp(ALL(rng));
    ITR(v, a) print(comp.rank(v) + 1);
    ITR(v, b) print(comp.rank(v) + 1);
}

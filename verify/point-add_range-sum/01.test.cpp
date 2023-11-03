/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include "template.hpp"
/* #endregion */

#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include "data_structure/segment_tree.hpp"
#include "data_structure/range_action/range_sum.hpp"


signed main() {
    int n, q; cin >> n >> q;
    vector<ll> a(n); input >> a;

    lib::segment_tree<lib::actions::range_sum<ll>> data(ALL(a));
    debug(data);

    REP(q) {
        int t; cin >> t;
        if(t == 0) {
            ll p, x; cin >> p >> x;
            data.set(p, data[p].val() + x);
        }
        if(t == 1) {
            int l, r; cin >> l >> r;
            print(data.fold(l, r));
        }
        debug(data);
    }

    return 0;
}

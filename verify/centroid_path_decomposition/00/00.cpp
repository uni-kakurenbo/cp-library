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

// https://atcoder.jp/contests/abc294/tasks/abc294_g

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

#include "graph.hpp"
#include "graph/centroid_path_decomposition.hpp"

#include "data_structure/fenwick_tree.hpp"
#include "data_structure/range_action/range_sum.hpp"

void solve() {
    int n; cin >> n;
    lib::graph G(n);

    valarray<std::tuple<int,int,i64>> es(n - 1);
    REP(i, n - 1) {
        i64 u, v, w; cin >> u >> v >> w; --u, --v;
        es[i] = { u, v, w };
        G.add_edge_bidirectionally(u, v);
    }

    lib::centroid_path_decomposition HLD(G);
    lib::fenwick_tree<lib::actions::range_sum<i64>> costs(n - 1);

    ITR(u, v, _w, es) {
        i64 w = _w;
        debug(u, v);
        HLD.path(u, v, [&](int l, int r) { debug(l, r); costs[l].add(w); });
    }
    debug(costs);

    int q; cin >> q;
    REP(q) {
        int t; cin >> t;
        if(t == 1) {
            i64 i, w; cin >> i >> w; --i;
            auto [ u, v, _ ] = es[i];
            HLD.path(u, v, [&](int l, int) { costs[l].set(w); });
        }
        if(t == 2) {
            int u, v; cin >> u >> v; --u, --v;
            debug(u, v);
            i64 res = 0;
            HLD.path(u, v, [&](int l, int r) { debug(l, r, costs(l, r).sum()); res += costs(l, r).sum(); });
            print(res);
        }
    }
}

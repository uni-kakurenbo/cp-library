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
/* #endregion */

#include "graph.hpp"
#include "graph/lowest_common_ancestor.hpp"

signed main() {
    int n, q; cin >> n >> q;
    lib::graph<> G(n);

    REP(i, 1, n) {
        int p; cin >> p;
        G.add_edge_bidirectionally(i, p);
    }

    lib::lowest_common_ancestor lca(G, 0);

    LOOP(q) {
        int u, v; cin >> u >> v;
        print(lca(u, v));
    }

    return 0;
}

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
#include "input.hpp"
#include "output.hpp"

input_adapter _input;
output_adapter _print;
#define input _input
#define print _print
/* #endregion */

#include "graph.hpp"
#include "graph/lowest_common_ancestor.hpp"

signed main() {
    int n; cin >> n;
    lib::graph<> G(n);

    REP(i, n) {
        int k; cin >> k;
        LOOP(k) {
            int c; cin >> c;
            G.add_edge_bidirectionally(i, c);
        }
    }

    lib::lowest_common_ancestor lca(G, 0);

    int q; cin >> q;
    LOOP(q) {
        int u, v; cin >> u >> v;
        print(lca(u, v));
    }

    return 0;
}

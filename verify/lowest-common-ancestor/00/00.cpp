/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
// #include <bits/stdc++.h>

// #include "template.hpp"
#include <iostream>

// #include "adapter/input.hpp"
#include "adapter/output.hpp"

output_adapter _print;
#define print _print
/* #endregion */

#include "graph.hpp"
#include "graph/lowest_common_ancestor.hpp"

signed main() {
    int n; std::cin >> n;
    lib::graph<> G(n);

    REP(i, n) {
        int k; std::cin >> k;
        LOOP(k) {
            int c; std::cin >> c;
            G.add_edge_bidirectionally(i, c);
        }
    }

    lib::lowest_common_ancestor lca(G, 0);

    int q; std::cin >> q;
    LOOP(q) {
        int u, v; std::cin >> u >> v;
        print(lca(u, v));
    }

    return 0;
}

#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_5_C"

#include "template.hpp"
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

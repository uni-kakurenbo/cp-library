// #include <bits/stdc++.h>
#include "template.hpp"
#include "graph.hpp"
#include "graph/manhattan_minimum_spanning_tree.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<ll> xs(n), ys(n);
    REP(i, n) std::cin >> xs[i] >> ys[i];

    ll cost = 0;
    auto edges = lib::manhattan_mst_edges(ALL(xs), ALL(ys), &cost);

    print(cost);
    ITR(u, v, w, edges) print(u, v);
}

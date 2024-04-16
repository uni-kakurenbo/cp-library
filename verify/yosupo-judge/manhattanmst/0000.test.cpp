#define PROBLEM "https://judge.yosupo.jp/problem/manhattanmst"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/vector.hpp"
#include "adaptor/io.hpp"
#include "graph/manhattan_minimum_spanning_tree.hpp"

signed main() {
    int n; std::cin >> n;
    uni::vector<uni::ll> xs(n), ys(n);
    REP(i, n) std::cin >> xs[i] >> ys[i];

    uni::ll cost = 0;
    auto edges = uni::manhattan_mst_edges(ALL(xs), ALL(ys), &cost);

    print(cost);
    ITR(u, v, w, edges) print(u, v);
}

#define PROBLEM "https://judge.yosupo.jp/problem/manhattanmst"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adapter/vector.hpp"
#include "adapter/io.hpp"
#include "graph/manhattan_minimum_spanning_tree.hpp"

signed main() {
    int n; std::cin >> n;
    lib::vector<lib::ll> xs(n), ys(n);
    REP(i, n) std::cin >> xs[i] >> ys[i];

    lib::ll cost = 0;
    auto edges = lib::manhattan_mst_edges(ALL(xs), ALL(ys), &cost);

    print(cost);
    ITR(u, v, w, edges) print(u, v);
}

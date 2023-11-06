/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc294/tasks/abc294_g"

#include <iostream>

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/valarray.hpp"
#include "graph.hpp"
#include "graph/centroid_path_decomposition.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/range_action/range_sum.hpp"

signed main() {
    int n; std::cin >> n;
    lib::graph G(n);

    lib::valarray<std::tuple<int, int, lib::i64>> es(n - 1);
    REP(i, n - 1) {
        int u, v, w; std::cin >> u >> v >> w; --u, --v;
        es[i] = { u, v, w };
        G.add_edge_bidirectionally(u, v);
    }

    lib::centroid_path_decomposition HLD(G);
    lib::fenwick_tree<lib::actions::range_sum<lib::i64>> costs(n - 1);

    ITR(u, v, _w, es) {
        lib::i64 w = _w;
        HLD.edges_on_path(u, v, [&](int l, [[maybe_unused]] int r) { costs[l] += w; });
    }

    int q; std::cin >> q;
    REP(q) {
        int t; std::cin >> t;
        if(t == 1) {
            int i, w; std::cin >> i >> w; --i;
            auto [ u, v, _ ] = es[i];
            HLD.edges_on_path(u, v, [&](int l, int) { costs[l] = w; });
        }
        if(t == 2) {
            int u, v; std::cin >> u >> v; --u, --v;
            lib::i64 res = 0;
            HLD.edges_on_path(u, v, [&](int l, int r) { res += costs(l, r).fold().val(); });
            print(res);
        }
    }
}

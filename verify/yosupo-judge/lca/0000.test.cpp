/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include <iostream>

#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "structure/graph.hpp"
#include "graph/lowest_common_ancestor.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::graph<> G(n);

    REP(i, 1, n) {
        uni::i32 p; input >> p;
        G.add_edge_bidirectionally(i, p);
    }

    uni::lowest_common_ancestor lca(G, 0);

    LOOP(q) {
        uni::i32 u, v; input >> u >> v;
        print(lca(u, v));
    }

    return 0;
}

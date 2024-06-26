/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "adaptor/vector.hpp"
#include "snippet/aliases.hpp"
#include "structure/graph.hpp"
#include "graph/shortest_path.hpp"

signed main() {
    uni::i32 n, m, s, t; input >> n >> m >> s >> t;
    uni::graph<uni::i64> graph(n); graph.read<true, false>(m);
    debug(graph);

    uni::vector<uni::i64> dist, prev;
    graph.shortest_path_with_cost(s, &dist, &prev);
    debug(dist, prev);

    if(dist[t] >= uni::INF64) {
        print(-1);
        return 0;
    }

    auto path = uni::restore_path(t, prev);
    debug(path);

    print(dist[t], path.size() - 1);

    REP(i, 1, path.size()) print(path[i - 1], path[i]);
}

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
#include "iterable/operation.hpp"
#include "data_structure/removable_priority_queue.hpp"

signed main() {
    uni::i32 n, m, s, t; input >> n >> m >> s >> t;
    uni::graph<uni::i64> graph(n); graph.read<true, false>(m);

    uni::removable_priority_queue<std::pair<uni::i64, uni::i32>, std::vector<std::pair<uni::i64, uni::i32>>, std::greater<std::pair<uni::i64, uni::i32>>> que;
    uni::vector<uni::i64> dists(n, uni::INF64);
    uni::vector<uni::i32> prev(n, -1);

    que.emplace(dists[s] = 0, s);

    while(!que.empty()) {
        auto [ d, v ] = que.top(); que.pop();

        ITR(e, graph[v]) {
            auto nd = d + e.cost;
            if(nd >= dists[e]) continue;

            if(dists[e] < uni::INF64) que.eliminate(dists[e], e);

            dists[e] = nd;
            prev[e] = v;
            que.emplace(nd, e.to);

        }
    }

    if(dists[t] >= uni::INF64) {
        print(-1);
        return 0;
    }

    auto path = uni::restore_path(t, prev);

    print(dists[t], path.size() - 1);
    REP(i, 1, path.size()) print(path[i - 1], path[i]);
}

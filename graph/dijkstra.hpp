/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
#include <vector>
#include <queue>
#include <utility>
#include <functional>

#include "template.hpp"
#include "graph.hpp"

template<class Cost, class Dist = Cost> void dijkstra(const Graph<Edge<Cost>> &G, const int s, std::vector<Dist> *dists) {
    using State = std::pair<Dist,int>;
    std::priority_queue<State,std::vector<State>,std::greater<State>> que;

    que.emplace(0, s), (*dists)[s] = 0;

    while(!que.empty()) {
        const auto [d, u] = que.top(); que.pop();

        if((*dists)[u] < d) continue;

        for(const auto &e : G[u]) {
            const int v = e.to; const Cost cost = e.cost;

            if((*dists)[v] <= d + cost) continue;

            (*dists)[v] = d + cost;
            que.emplace((*dists)[v], v);
        }
    }

    return;
}

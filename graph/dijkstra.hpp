#pragma once

#include <vector>
#include <queue>
#include <utility>
#include <functional>

#include "snippet/iterations.hpp"
#include "graph.hpp"

template<class E>
template<class dist_t>
void lib::graph<E>::dijkstra(const vertex s, std::vector<dist_t> *dists) const {
    using state = std::pair<dist_t,vertex>;
    std::priority_queue<state,std::vector<state>,std::greater<state>> que;

    dists->assign(this->size(), std::numeric_limits<dist_t>::max());

    que.emplace(0, s), (*dists)[s] = 0;

    while(not que.empty()) {
        const auto [d, u] = que.top(); que.pop();

        if((*dists)[u] < d) continue;

        ITR(e, (*this)[u]) {
            const vertex v = e.to; const auto cost = e.cost;

            if((*dists)[v] <= d + cost) continue;

            (*dists)[v] = d + cost;
            que.emplace((*dists)[v], v);
        }
    }

    return;
}

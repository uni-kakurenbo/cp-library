#pragma once

#include <vector>
#include <queue>
#include <utility>
#include <functional>

#include "snippet/iterations.hpp"
#include "graph.hpp"

template<class edge_cost>
template<class cost_type>
void lib::graph<edge_cost>::dijkstra(const vertex s, std::vector<cost_type> *dists) const {
    using state = std::pair<cost_type,vertex>;
    std::priority_queue<state,std::vector<state>,std::greater<state>> que;

    dists->assign(this->size(), std::numeric_limits<cost_type>::max());

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

#pragma once

#include <vector>
#include <queue>
#include <utility>
#include <functional>

#include "template.hpp"
#include "graph.hpp"

template<class E>
template<class Dist>
void Graph<E>::dijkstra(const Vertex s, std::vector<Dist> *dists) const {
    using State = std::pair<Dist,Vertex>;
    std::priority_queue<State,std::vector<State>,std::greater<State>> que;

    que.emplace(0, s), (*dists)[s] = 0;

    until(que.empty()) {
        const auto [d, u] = que.top(); que.pop();

        if((*dists)[u] < d) continue;

        ITR(e, (*this)[u]) {
            const Vertex v = e.to; const auto cost = e.cost;

            if((*dists)[v] <= d + cost) continue;

            (*dists)[v] = d + cost;
            que.emplace((*dists)[v], v);
        }
    }

    return;
}

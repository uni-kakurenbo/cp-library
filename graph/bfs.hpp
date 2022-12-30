#pragma once

#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
#include "snippet/internal/types.hpp"

#include "graph.hpp"

template<class E>
template<class dist_t>
void lib::graph<E>::bfs(const vertex s, std::vector<dist_t> *dists) const {
    dists->assign(this->size(), -1);

    std::queue<vertex> que;
    que.push(s), (*dists)[s] = 0;

    while(not que.empty()) {
        const vertex v = que.front(); que.pop();
        ITR(nv, (*this)[v]) {
            if((*dists)[nv.to] >= 0) continue;
            (*dists)[nv.to] = (*dists)[v] + 1;
            que.push(nv.to);
        }
    }
}

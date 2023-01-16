#pragma once


#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
#include "snippet/internal/types.hpp"

#include "graph.hpp"


template<class edge_cost>
template<class cost_type>
void lib::graph<edge_cost>::bfs(const vertex s, std::vector<cost_type> *dists) const {
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

template<class edge_cost>
template<class cost_type>
std::vector<cost_type> lib::graph<edge_cost>::bfs(const vertex s) const {
    std::vector<cost_type> dists;
    this->bfs<cost_type>(s, &dists);
    return dists;
}

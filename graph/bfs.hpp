#pragma once


#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
#include "snippet/internal/types.hpp"

#include "graph.hpp"


template<class edge_cost>
template<class cost_t>
void lib::graph<edge_cost>::bfs(const size_type s, std::vector<cost_t> *dists) const {
    dists->assign(this->size(), -1);

    std::queue<size_type> que;
    que.push(s), (*dists)[s] = 0;

    while(not que.empty()) {
        const size_type v = que.front(); que.pop();
        ITR(nv, (*this)[v]) {
            if((*dists)[nv.to] >= 0) continue;
            (*dists)[nv.to] = (*dists)[v] + 1;
            que.push(nv.to);
        }
    }
}

template<class edge_cost>
template<class cost_t>
std::vector<cost_t> lib::graph<edge_cost>::bfs(const size_type s) const {
    std::vector<cost_t> dists;
    this->bfs<cost_t>(s, &dists);
    return dists;
}

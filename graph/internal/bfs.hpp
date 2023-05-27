#pragma once


#include <vector>
#include <queue>

#include "internal/dev_env.hpp"

#include "snippet/iterations.hpp"
#include "snippet/internal/types.hpp"

#include "graph.hpp"


template<class edge_cost>
template<class cost_t>
void lib::graph<edge_cost>::distances_without_cost(const size_type s, std::vector<cost_t> *dists) const noexcept(DEV_ENV) {
    dists->assign(this->size(), std::numeric_limits<cost_t>::max());

    std::queue<size_type> que;
    que.push(s), (*dists)[s] = 0;

    while(not que.empty()) {
        const size_type v = que.front(); que.pop();
        ITR(nv, (*this)[v]) {
            if((*dists)[nv.to] < std::numeric_limits<cost_t>::max()) continue;
            (*dists)[nv.to] = (*dists)[v] + 1;
            que.push(nv.to);
        }
    }
}

template<class edge_cost>
template<class cost_t>
std::vector<cost_t> lib::graph<edge_cost>::distances_without_cost(const size_type s) const noexcept(DEV_ENV) {
    std::vector<cost_t> dists;
    this->distances_without_cost<cost_t>(s, &dists);
    return dists;
}

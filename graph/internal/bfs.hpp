#pragma once


#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
#include "snippet/internal/types.hpp"

#include "internal/dev_env.hpp"

#include "numeric/limits.hpp"
#include "graph.hpp"


template<class edge_cost>
template<class cost_t>
void lib::graph<edge_cost>::distances_without_cost(const size_type s, std::vector<cost_t> *dists) const noexcept(NO_EXCEPT) {
    dists->assign(this->size(), lib::numeric_limits<cost_t>::arithmetic_infinity());

    std::queue<size_type> que;
    que.push(s), (*dists)[s] = 0;

    while(not que.empty()) {
        const size_type v = que.front(); que.pop();
        ITR(nv, (*this)[v]) {
            if((*dists)[nv.to] < lib::numeric_limits<cost_t>::arithmetic_infinity()) continue;
            (*dists)[nv.to] = (*dists)[v] + 1;
            que.push(nv.to);
        }
    }
}

template<class edge_cost>
template<class cost_t>
std::vector<cost_t> lib::graph<edge_cost>::distances_without_cost(const size_type s) const noexcept(NO_EXCEPT) {
    std::vector<cost_t> dists;
    this->distances_without_cost<cost_t>(s, &dists);
    return dists;
}

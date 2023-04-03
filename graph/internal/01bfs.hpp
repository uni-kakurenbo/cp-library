#pragma once


#include <vector>
#include <deque>
#include <utility>
#include <functional>

#include "snippet/iterations.hpp"
#include "graph.hpp"


template<class edge_cost>
template<class cost_t>
void lib::graph<edge_cost>::distances_with_01cost(const size_type s, std::vector<cost_t> *const dists) const {
    std::deque<size_type> que;

    dists->assign(this->size(), std::numeric_limits<cost_t>::max());
    que.push_back(s), (*dists)[s] = 0;

    while(not que.empty()) {
        const auto u = que.front(); que.pop_front();
        const cost_t d = (*dists)[u];

        ITR(e, (*this)[u]) {
            const size_type v = e.to; const auto cost = e.cost;

            if((*dists)[v] <= d + cost) continue;

            (*dists)[v] = d + cost;

            if(cost) que.push_back(v);
            else que.push_front(v);
        }
    }
}

template<class edge_cost>
template<class cost_t>
std::vector<cost_t> lib::graph<edge_cost>::distances_with_01cost(const size_type s) const {
    std::vector<cost_t> dists;
    this->distances_with_01cost<cost_t>(s, &dists);
    return dists;
}

#pragma once

#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
#include "graph.hpp"

template<class edge_cost>
template<class comparer>
bool lib::graph<edge_cost>::sort_topologically_with_priority(std::vector<vertex> *sorted) const {
    sorted->clear();

    std::vector<vertex> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::priority_queue<vertex,std::vector<vertex>,comparer> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    while(not que.empty()) {
        const vertex v = que.top(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

template<class edge_cost>
bool lib::graph<edge_cost>::sort_topologically(std::vector<vertex> *sorted) const {
    sorted->clear();

    std::vector<vertex> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::queue<vertex> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    while(not que.empty()) {
        const vertex v = que.front(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

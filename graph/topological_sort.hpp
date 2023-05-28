#pragma once


#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
#include "internal/dev_env.hpp"
#include "graph.hpp"


template<class edge_cost>
template<class comparer>
bool lib::graph<edge_cost>::sort_topologically_with_priority(std::vector<size_type> *const sorted) const noexcept(NO_EXCEPT) {
    sorted->clear();

    std::vector<size_type> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::priority_queue<size_type,std::vector<size_type>,comparer> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    while(not que.empty()) {
        const size_type v = que.top(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

template<class edge_cost>
template<class comparer>
bool lib::graph<edge_cost>::sort_topologically_with_priority() const noexcept(NO_EXCEPT) {
    std::vector<size_type> vs;
    return this->sort_topologically_with_priority<comparer>(&vs);
}


template<class edge_cost>
bool lib::graph<edge_cost>::sort_topologically(std::vector<size_type> *const sorted) const noexcept(NO_EXCEPT) {
    sorted->clear();

    std::vector<size_type> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::queue<size_type> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    while(not que.empty()) {
        const size_type v = que.front(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

template<class edge_cost>
bool lib::graph<edge_cost>::sort_topologically() const noexcept(NO_EXCEPT) {
    std::vector<size_type> vs;
    return this->sort_topologically(&vs);
}

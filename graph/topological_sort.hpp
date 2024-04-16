#pragma once


#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
#include "internal/dev_env.hpp"
#include "structure/graph.hpp"


template<class Graph>
template<class comparer>
bool uni::internal::graph_impl::mixin<Graph>::sort_topologically_with_priority(uni::vector<node_type> *const sorted) const noexcept(NO_EXCEPT) {
    sorted->clear();

    std::vector<size_type> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::priority_queue<node_type,std::vector<node_type>,comparer> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    while(not que.empty()) {
        const node_type v = que.top(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

template<class Graph>
template<class comparer>
bool uni::internal::graph_impl::mixin<Graph>::sort_topologically_with_priority() const noexcept(NO_EXCEPT) {
    uni::vector<node_type> vs;
    return this->sort_topologically_with_priority<comparer>(&vs);
}


template<class Graph>
bool uni::internal::graph_impl::mixin<Graph>::sort_topologically(uni::vector<node_type> *const sorted) const noexcept(NO_EXCEPT) {
    sorted->clear();

    std::vector<size_type> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::queue<node_type> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    while(not que.empty()) {
        const node_type v = que.front(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

template<class Graph>
bool uni::internal::graph_impl::mixin<Graph>::sort_topologically() const noexcept(NO_EXCEPT) {
    std::vector<node_type> vs;
    return this->sort_topologically(&vs);
}

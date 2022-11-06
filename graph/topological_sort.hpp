#pragma once

#include <vector>
#include <queue>

#include "template.hpp"
#include "graph.hpp"

template<class E>
template<class Comparer>
bool Lib::Graph<E>::sort_topologically_with_priority(std::vector<Vertex> *sorted) const {
    sorted->clear();

    std::vector<Vertex> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::priority_queue<Vertex,std::vector<Vertex>,Comparer> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    while(!que.empty()) {
        const Vertex v = que.top(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

template<class E>
bool Lib::Graph<E>::sort_topologically(std::vector<Vertex> *sorted) const {
    sorted->clear();

    std::vector<Vertex> in_degs(this->size());
    ITR(v, *this) ITR(e, v)  ++in_degs[e.to];

    std::queue<Vertex> que;
    REP(i, this->size()) if(in_degs[i] == 0) que.push(i);

    until(que.empty()) {
        const Vertex v = que.front(); que.pop();
        ITR(u, (*this)[v]) if(!(--in_degs[u.to])) que.push(u.to);
        sorted->push_back(v);
    }

    return sorted->size() == this->size();
}

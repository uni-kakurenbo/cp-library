#pragma once

#include <vector>
#include <queue>

#include "template.hpp"
#include "graph.hpp"

template<class E>
template<class Dist>
void Graph<E>::bfs(const Vertex s, std::vector<Dist> *dists) const {
    dists->assign(this->size(), -1);

    std::queue<Vertex> que;
    que.push(s), (*dists)[s] = 0;

    until(que.empty()) {
        const Vertex v = que.front(); que.pop();
        ITR(nv, (*this)[v]) {
            if((*dists)[nv.to] >= 0) continue;
            (*dists)[nv.to] = (*dists)[v] + 1;
            que.push(nv.to);
        }
    }
}

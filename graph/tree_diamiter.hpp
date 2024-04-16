#pragma once


#include <utility>
#include <vector>

#include "structure/graph.hpp"


namespace uni {

namespace internal {


template<class Graph>
std::pair<typename Graph::cost_type, typename Graph::node_type> farthest(const Graph& tree, const typename Graph::node_type v, const typename Graph::node_type p, std::vector<typename Graph::node_type> *const prev = nullptr) {
    std::pair<typename Graph::cost_type, typename Graph::node_type> res = { 0, v };

    for(auto nv : tree[v]) {
        if(nv.to == p) continue;

        auto next = farthest(tree, nv.to, v, prev);
        next.first += nv.cost;

        if(res.first < next.first) {
            if(prev) prev->operator[](nv.to) = v;
            res = next;
        }
    }

    return res;
}


} // namespace internal


template<class Graph>
auto tree_diamiter(const Graph& tree, std::vector<typename Graph::node_type> *const prev = nullptr) {
    const auto p = farthest(tree, 0, -1);
    return farthest(tree, p.second, -1, prev);
}



} // namespace uni

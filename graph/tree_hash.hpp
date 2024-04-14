#pragma once


#include <utility>
#include <vector>

#include "snippet/aliases.hpp"
#include "graph/tree_diamiter.hpp"


namespace lib {


template<class Graph>
auto tree_centers(const Graph& tree) {
    std::vector<typename Graph::node_type> prev(std::ranges::size(tree), -1);
    auto [ diam, v ] = tree_diamiter(tree, &prev);

    std::vector<typename Graph::node_type> res;

    {
        for(typename Graph::size_type i = 0; i < diam / 2; ++i) {
            v = prev[v];
        }
        res.push_back(v);
        if(diam % 2 == 1) res.push_back(prev[v]);
    }

    {
        auto rest = std::ranges::unique(res);
        res.erase(std::ranges::begin(rest), std::ranges::end(rest));
    }

    return std::make_pair(diam, res);
}


template<class Graph>
std::size_t tree_hash(const Graph& tree, typename Graph::node_type v, typename Graph::node_type p = -1) {
    static std::map<std::vector<typename Graph::node_type>, std::size_t> vals;

    std::vector<typename Graph::node_type> children;

    for(const auto nv : tree[v]) {
        if(nv == p) continue;
        children.emplace_back(tree_hash(tree, nv, v));
    }

    std::ranges::sort(children);

    if(!vals.contains(children)) {
        vals[children] = 0;
        vals[children] = std::ranges::size(vals);
    }

    return vals[children];
}


} // namespace lib

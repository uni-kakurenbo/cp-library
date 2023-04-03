#pragma once

#include <atcoder/dsu>

#include "graph.hpp"

namespace lib {

namespace internal {

namespace graph_lib {


template<class G, class cost_type, class Compare>
cost_type kruskal(const G& graph, const Compare compare) {
    atcoder::dsu ds(graph.size());

    std::vector<std::tuple<typename G::edge::cost_type,size_t,size_t>> edges;

    REP(u, graph.size()) ITR(e, graph[u]) {
        edges.emplace_back(e.cost, u, e.to);
    }

    std::sort(ALL(edges), compare);

    cost_type res = cost_type{};
    ITR(w, u, v, edges) {
        if(not ds.same(u, v)) {
            ds.merge(u, v);
            res += w;
        }
    }

    return res;
}


} // namespace graph_lib

} // namespace internal


template<class edge_cost>
template<class cost_type>
cost_type graph<edge_cost>::minimum_spanning_tree_cost() const {
    return internal::graph_lib::kruskal<graph<edge_cost>,cost_type,std::less<tuple<cost_type,size_type,size_type>>>(*this, {});
}

template<class edge_cost>
template<class cost_type>
inline cost_type graph<edge_cost>::maximum_spanning_tree_cost() const {
    return internal::graph_lib::kruskal<graph<edge_cost>,cost_type,std::greater<tuple<cost_type,size_type,size_type>>>(*this, {});
}


} // namespace lib

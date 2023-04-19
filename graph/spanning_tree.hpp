#pragma once

#include <atcoder/dsu>

#include "graph.hpp"

namespace lib {

namespace internal {

namespace graph_impl {


template<class G, class cost_type, class Compare>
cost_type kruskal(const G& graph, const Compare compare, G *const mst = nullptr) {
    atcoder::dsu ds(graph.size());

    std::vector<std::tuple<typename G::edge::cost_type,size_t,size_t>> edges;

    REP(u, graph.size()) ITR(e, graph[u]) {
        edges.emplace_back(e.cost, u, e.to);
    }

    std::sort(ALL(edges), compare);

    if(mst) mst->clear(), mst->resize(graph.size());

    cost_type res = cost_type{};
    ITR(w, u, v, edges) {
        if(not ds.same(u, v)) {
            ds.merge(u, v);
            if(mst) mst->add_edge_bidirectionally(u, v, w);
            res += w;
        }
    }

    return res;
}


} // namespace graph_impl

} // namespace internal


template<class edge_cost>
template<class cost_type>
cost_type graph<edge_cost>::minimum_spanning_tree(graph<edge_cost> *const mst) const {
    return internal::graph_impl::kruskal<graph<edge_cost>,cost_type,std::less<tuple<cost_type,size_type,size_type>>>(*this, {}, mst);
}

template<class edge_cost>
template<class cost_type>
inline cost_type graph<edge_cost>::maximum_spanning_tree(graph<edge_cost> *const mst) const {
    return internal::graph_impl::kruskal<graph<edge_cost>,cost_type,std::greater<tuple<cost_type,size_type,size_type>>>(*this, {}, mst);
}


} // namespace lib

#pragma once

#include <atcoder/dsu>

#include "internal/dev_env.hpp"
#include "structure/graph.hpp"

namespace lib {

namespace internal {

namespace graph_impl {


template<class G, class cost_type, class Compare>
cost_type kruskal(const G& graph, const Compare compare, G *const mst = nullptr) noexcept(NO_EXCEPT) {
    atcoder::dsu ds(graph.size());

    std::vector<std::tuple<typename G::edge_type::cost_type,size_t,size_t>> edges;

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


template<class Graph>
template<class cost_type>
cost_type internal::graph_impl::mixin<Graph>::minimum_spanning_tree(internal::graph_impl::mixin<Graph> *const mst) const noexcept(NO_EXCEPT) {
    return internal::graph_impl::kruskal<internal::graph_impl::mixin<Graph>,cost_type,std::less<std::tuple<cost_type,size_t,size_t>>>(*this, {}, mst);
}

template<class Graph>
template<class cost_type>
inline cost_type internal::graph_impl::mixin<Graph>::maximum_spanning_tree(internal::graph_impl::mixin<Graph> *const mst) const noexcept(NO_EXCEPT) {
    return internal::graph_impl::kruskal<internal::graph_impl::mixin<Graph>,cost_type,std::greater<std::tuple<cost_type,size_t,size_t>>>(*this, {}, mst);
}


} // namespace lib

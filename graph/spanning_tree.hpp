#pragma once


#include <atcoder/dsu>


#include "internal/dev_env.hpp"
#include "structure/graph.hpp"


namespace lib {

namespace internal {

namespace graph_impl {


template<class G, template<class...> class Compare, class Cost, class Size>
auto kruskal(const G& graph, const Compare<std::tuple<Cost, Size, Size>> compare, G *const mst = nullptr) noexcept(NO_EXCEPT) {
    atcoder::dsu ds(graph.size());

    std::vector<std::tuple<Cost, Size, Size>> edges;

    REP(u, graph.size()) ITR(e, graph[u]) {
        edges.emplace_back(e.cost, u, e.to);
    }

    std::ranges::sort(ALL(edges), compare);

    if(mst) mst->clear(), mst->resize(graph.size());

    Cost res = {};

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

} // namespace lib


template<class Graph>
inline auto lib::internal::graph_impl::mixin<Graph>::minimum_spanning_tree(internal::graph_impl::mixin<Graph> *const mst) const noexcept(NO_EXCEPT) {
    return
        lib::internal::graph_impl::kruskal<
            lib::internal::graph_impl::mixin<Graph>,
            std::less<std::tuple<cost_type, size_type, size_type>>
        >(*this, {}, mst);
}

template<class Graph>
inline auto lib::internal::graph_impl::mixin<Graph>::maximum_spanning_tree(internal::graph_impl::mixin<Graph> *const mst) const noexcept(NO_EXCEPT) {
    return
        lib::internal::graph_impl::kruskal<
            lib::internal::graph_impl::mixin<Graph>,
            std::greater<std::tuple<cost_type, size_type, size_type>>
        >(*this, {}, mst);
}

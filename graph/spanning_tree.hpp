#pragma once


#include <atcoder/dsu>


#include "internal/dev_env.hpp"
#include "structure/graph.hpp"


namespace uni {

namespace internal {

namespace graph_impl {


template<class G, template<class...> class Compare, class Cost, class Size>
std::optional<Cost> kruskal(const G& graph, const Compare<std::tuple<Cost, Size, Size>> compare, G *const mst = nullptr) noexcept(NO_EXCEPT) {
    atcoder::dsu ds(graph.size());

    std::vector<std::tuple<Cost, Size, Size>> edges;

    REP(u, graph.size()) ITR(e, graph[u]) {
        edges.emplace_back(e.cost, u, e.to);
    }

    std::ranges::sort(ALL(edges), compare);

    if(mst) mst->clear(), mst->resize(graph.size());

    Cost res = {};

    typename G::size_type cnt = 0;
    ITR(w, u, v, edges) {
        if(not ds.same(u, v)) {
            ds.merge(u, v);
            if(mst) mst->add_edge_bidirectionally(u, v, w);
            res += w;
            ++cnt;
        }
    }

    assert(cnt <= graph.size() - 1);
    if(cnt != graph.size() - 1) return {};

    return res;
}


} // namespace graph_impl

} // namespace internal

} // namespace uni


template<class Graph>
inline auto uni::internal::graph_impl::mixin<Graph>::minimum_spanning_tree(internal::graph_impl::mixin<Graph> *const mst) const noexcept(NO_EXCEPT) {
    return
        uni::internal::graph_impl::kruskal<
            uni::internal::graph_impl::mixin<Graph>,
            std::less, cost_type, size_type
        >(*this, {}, mst);
}

template<class Graph>
inline auto uni::internal::graph_impl::mixin<Graph>::maximum_spanning_tree(internal::graph_impl::mixin<Graph> *const mst) const noexcept(NO_EXCEPT) {
    return
        uni::internal::graph_impl::kruskal<
            uni::internal::graph_impl::mixin<Graph>,
            std::less, cost_type, size_type
        >(*this, {}, mst);
}

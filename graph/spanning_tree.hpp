#pragma once

#include <atcoder/dsu>

#include "graph.hpp"

namespace lib {

namespace internal {

namespace graph_lib {


template<class G, class cost_t, class Compare> cost_t kruskal(const G& graph, Compare compare) {
    atcoder::dsu uft(graph.size());

    std::vector<std::tuple<typename G::edge::cost_type,typename G::vertex,typename G::vertex>> edges;

    REP(u, graph.size()) ITR(e, graph[u]) {
        edges.emplace_back(e.cost, u, e.to);
    }

    std::sort(ALL(edges), compare);

    cost_t res = cost_t{};
    ITR(w, u, v, edges) {
        if(not uft.same(u, v)) {
            uft.merge(u, v);
            res += w;
        }
    }

    return res;
}


} // namespace graph_lib

} // namespace internal


template<class E>
template<class cost_t>
cost_t graph<E>::minimum_spanning_tree_cost() const {
    return internal::graph_lib::kruskal<graph<E>,cost_t,std::less<tuple<cost_t,vertex,vertex>>>(*this, {});
}

template<class E>
template<class cost_t>
inline cost_t graph<E>::maximum_spanning_tree_cost() const {
    return internal::graph_lib::kruskal<graph<E>,cost_t,std::greater<tuple<cost_t,vertex,vertex>>>(*this, {});
}


} // namespace lib

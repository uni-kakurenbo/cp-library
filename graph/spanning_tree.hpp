#pragma once

#include <atcoder/dsu>

#include "graph.hpp"

namespace Lib {

namespace GraphLib {

template<class G, class Cost, class Compare> Cost kruskal(const G& graph, Compare compare) {
    atcoder::dsu uft(graph.size());

    std::vector<std::tuple<typename G::Edge::cost_type,typename G::Vertex,typename G::Vertex>> edges;

    REP(u, graph.size()) ITR(e, graph[u]) {
        edges.emplace_back(e.cost, u, e.to);
    }

    std::sort(ALL(edges), compare);

    Cost res = Cost{};
    ITR(w, u, v, edges) {
        if(not uft.same(u, v)) {
            uft.merge(u, v);
            res += w;
        }
    }

    return res;
}

} // namespace GraphLib

template<class E>
template<class Cost>
Cost Graph<E>::minimum_spanning_tree_cost() const {
    return GraphLib::kruskal<Graph<E>,Cost,std::less<tuple<Cost,Vertex,Vertex>>>(*this, {});
}

template<class E>
template<class Cost>
inline Cost Graph<E>::maximum_spanning_tree_cost() const {
    return GraphLib::kruskal<Graph<E>,Cost,std::greater<tuple<Cost,Vertex,Vertex>>>(*this, {});
}


} // namespace Lib

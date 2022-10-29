#pragma once

#include <utility>

#include "template.hpp"

#include "develop/dev_assert.hpp"

template<class Cost = ll> struct Edge {
    using cost_type = Cost;
    const int to; const Cost cost;

    Edge(const int t, const Cost w) : to(t), cost(w) {}

    std::pair<int, Cost> _debug() const { return { to, cost }; };
};

template<class E = Edge<ll>> struct Graph : std::vector<std::vector<E>> {
    enum class EdgeType { Undirected, Directed };

    Graph(const int n = 0) : std::vector<std::vector<E>>(n) {}

    template<const EdgeType EDGE_TYPE = EdgeType::Directed>
    void add_edge(const int u, const int v, const typename E::cost_type w = 0) {
        dev_assert(0 <= u and u < (int)this->size()), dev_assert(0 <= v and v < (int)this->size());
        (*this)[u].emplace_back(v, w);
        if constexpr(EDGE_TYPE == EdgeType::Undirected) {
            (*this)[v].emplace_back(u, w);
        }
    }

    // graph/bfs.hpp
    template<class Dist = typename E::cost_type>
    void bfs(const int, std::vector<Dist>*) const;

    // graph/dijkstra.hpp
    template<class Dist = typename E::cost_type>
    void dijkstra(const int, std::vector<Dist>*) const;

    // graph/dijkstra.hpp
    template<class>
    bool sort_topologically_with_priority(std::vector<int>*) const;
    bool sort_topologically(std::vector<int>*) const;

    // graph/from_grid.hpp
    template<class G, class U = char>
    void from_grid(G*, U = '.');
};

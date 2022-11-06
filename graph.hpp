#pragma once

#include <utility>

#include "template.hpp"

#include "internal/develop/dev_assert.hpp"
#include "internal/types.hpp"

template<class Cost = ll> struct Edge {
    using cost_type = Cost;
    const Internal::Size to; const Cost cost;

    Edge(const Internal::Size t, const Cost w) : to(t), cost(w) {}

    std::pair<Internal::Size, Cost> _debug() const { return { to, cost }; };
};

template<class E = Edge<ll>> struct Graph : std::vector<std::vector<E>> {
    using Vertex = Internal::Size;

    enum class EdgeType { Undirected, Directed };

    Graph(const Vertex n = 0) : std::vector<std::vector<E>>(n) {}

    template<const EdgeType EDGE_TYPE = EdgeType::Directed>
    void add_edge(const Vertex u, const Vertex v, const typename E::cost_type w = 0) {
        dev_assert(0 <= u and u < (Vertex)this->size()), dev_assert(0 <= v and v < (Vertex)this->size());
        (*this)[u].emplace_back(v, w);
        if constexpr(EDGE_TYPE == EdgeType::Undirected) {
            (*this)[v].emplace_back(u, w);
        }
    }

    // graph/bfs.hpp
    template<class Dist = typename E::cost_type>
    void bfs(const Vertex, std::vector<Dist>*) const;

    // graph/dijkstra.hpp
    template<class Dist = typename E::cost_type>
    void dijkstra(const Vertex, std::vector<Dist>*) const;

    // graph/dijkstra.hpp
    template<class>
    bool sort_topologically_with_priority(std::vector<Vertex>*) const;
    bool sort_topologically(std::vector<Vertex>*) const;

    // graph/from_grid.hpp
    template<class G, class U = char>
    void from_grid(const G&, U = '.');
};

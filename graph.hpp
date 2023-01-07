#pragma once

#include <utility>
#include <vector>

#include "snippet/internal/types.hpp"

#include "internal/dev_assert.hpp"
#include "internal/types.hpp"

namespace lib {

template<class cost_t = ll> struct edge {
    using cost_type = cost_t;
    const internal::size_t to; const cost_t cost;

    edge(const internal::size_t t, const cost_t w) : to(t), cost(w) {}

    std::pair<internal::size_t, cost_t> _debug() const { return { to, cost }; };
};

template<class E = edge<ll>> struct graph : std::vector<std::vector<E>> {
    using vertex = internal::size_t;
    using edge = E;

    enum class edge_type { undirected, directed };

    graph(const vertex n = 0) : std::vector<std::vector<E>>(n) {}

    template<const edge_type EDGE_TYPE = edge_type::directed>
    inline void add_edge(const vertex u, const vertex v, const typename E::cost_type w = 0) {
        dev_assert(0 <= u and u < (vertex)this->size()), dev_assert(0 <= v and v < (vertex)this->size());
        (*this)[u].emplace_back(v, w);
        if constexpr(EDGE_TYPE == edge_type::undirected) {
            (*this)[v].emplace_back(u, w);
        }
    }

    inline void add_edge_bidirectionally(const vertex u, const vertex v, const typename E::cost_type w = 0) {
        this->add_edge<edge_type::undirected>(u, v, w);
    }

    // graph/bfs.hpp
    template<class dist_t = typename E::cost_type>
    inline void bfs(const vertex, std::vector<dist_t>*) const;

    // graph/dijkstra.hpp
    template<class dist_t = typename E::cost_type>
    inline void dijkstra(const vertex, std::vector<dist_t>*) const;

    // graph/dijkstra.hpp
    template<class>
    inline bool sort_topologically_with_priority(std::vector<vertex>*) const;
    inline bool sort_topologically(std::vector<vertex>*) const;

    // graph/minimum_paph_cover.hpp
    inline vertex minimum_paph_cover_size_as_dag() const;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = typename E::cost_type>
    inline cost_t minimum_spanning_tree_cost() const;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = typename E::cost_type>
    inline cost_t maximum_spanning_tree_cost() const;

    // graph/from_grid.hpp
    template<class G, class U = char>
    inline void from_grid(const G&, U = '.');

    // graph/manhattan_minimum_spanning_tree.hpp
    template<class distance_type = typename E::cost_type, class = internal::size_t>
    inline distance_type build_manhattan_mst(const std::vector<distance_type>&, const std::vector<distance_type>&);
};

} // namespace lib

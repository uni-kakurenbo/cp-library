#pragma once

#include <tuple>
#include <vector>

#include "snippet/internal/types.hpp"

#include "internal/dev_assert.hpp"
#include "internal/types.hpp"

namespace lib {

namespace internal {

namespace graph_lib {


template<class cost_t = ll, class vertex = internal::size_t> struct edge {
    using cost_type = cost_t;
    const vertex from, to; const cost_t cost;

    edge(const vertex u, const vertex v, const cost_t w) : from(u), to(v), cost(w) {}

    std::tuple<vertex,vertex,cost_t> _debug() const { return { from, to, cost }; };
};


} // namespace graph_lib

} // namespace internal

template<class cost_type = ll>
struct graph : std::vector<std::vector<internal::graph_lib::edge<cost_type>>> {
    using vertex = internal::size_t;
    using size_type = internal::size_t;

    using edge = typename internal::graph_lib::edge<cost_type>;

    enum class edge_type { undirected, directed };

  private:
    size_type _directed_edge_count, _undirected_edge_count;

  protected:
    inline void _add_edge(const vertex u, const vertex v, const cost_type w) {
        this->operator[](u).emplace_back(u, v, w);
        ++this->_directed_edge_count;
    }

  public:
    explicit graph(const vertex n = 0) : std::vector<std::vector<edge>>(n) {}

    inline vertex vertexes() const { return this->size(); }
    inline size_type edges() const { return this->inputted.size(); }

    template<const edge_type EDGE_TYPE = edge_type::directed>
    inline void add_edge(const vertex u, const vertex v, const cost_type w = 0) {
        dev_assert(0 <= u and u < this->vertexes()), dev_assert(0 <= v and v < this->vertexes());
        this->_add_edge(u, v, w);
        if constexpr(EDGE_TYPE == edge_type::undirected) this->_add_edge(v, u, w);
        ++this->_undirected_edge_count;
    }

    inline void add_edge_bidirectionally(const vertex u, const vertex v, const cost_type w = 0) {
        this->add_edge<edge_type::undirected>(u, v, w);
    }

    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, const edge_type EDGE_TYPE = edge_type::directed, class Stream = std::istream>
    void inline read(const size_type edges, Stream *const ist = &std::cin) {
        REP(edges) {
            vertex u, v; cost_type w = 0; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<EDGE_TYPE>(u, v, w);
        }
    }
    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, class Stream = std::istream>
    void inline read_bidirectionally(const size_type edges, Stream *const ist = &std::cin) {
        REP(edges) {
            vertex u, v; cost_type w = 0; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<edge_type::undirected>(u, v, w);
        }
    }

    // graph/bfs.hpp
    template<class cost_t = cost_type> inline void bfs(const vertex, std::vector<cost_t> *const) const;
    template<class cost_t = cost_type> inline std::vector<cost_t> bfs(const vertex) const;

    // graph/dijkstra.hpp
    template<class cost_t = cost_type> inline void dijkstra(const vertex, std::vector<cost_t> *const) const;
    template<class cost_t = cost_type> inline std::vector<cost_t> dijkstra(const vertex) const;

    // graph/topological_sort.hpp
    inline bool sort_topologically(std::vector<vertex> *const ) const;
    inline bool sort_topologically() const;

    // graph/topological_sort.hpp
    template<class> inline bool sort_topologically_with_priority(std::vector<vertex> *const) const;
    template<class> inline bool sort_topologically_with_priority() const;

    // graph/minimum_paph_cover.hpp
    inline vertex minimum_paph_cover_size_as_dag() const;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = cost_type>
    inline cost_t minimum_spanning_tree_cost() const;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = cost_type>
    inline cost_t maximum_spanning_tree_cost() const;

    // graph/manhattan_minimum_spanning_tree.hpp
    inline vertex count_components() const;

    // graph/from_grid.hpp
    template<class G, class U = char>
    inline void from_grid(const G&, U = '.');

    // graph/manhattan_minimum_spanning_tree.hpp
    template<class distance_type = cost_type, class = internal::size_t>
    inline distance_type build_manhattan_mst(const std::vector<distance_type>&, const std::vector<distance_type>&);
};

} // namespace lib

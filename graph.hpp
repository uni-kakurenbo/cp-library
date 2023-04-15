#pragma once

#include <cassert>
#include <tuple>
#include <vector>
#include <iostream>

#include "snippet/internal/types.hpp"

#include "internal/types.hpp"
#include "snippet/iterations.hpp"

#include "data_structure/disjoint_set_union.hpp"


namespace lib {

namespace internal {

namespace graph_impl {


template<class cost_t, class size_type> struct edge {
  private:
    inline static internal::size_t unique() { static internal::size_t id = 0; return id++; }

  public:
    using cost_type = cost_t;

    const internal::size_t id = unique();
    const size_type from, to; const cost_t cost;

    edge(const size_type u, const size_type v, const cost_t w) : from(u), to(v), cost(w) {}

    std::tuple<size_type,size_type,cost_t> _debug() const { return { from, to, cost }; };

    friend bool operator==(const edge& lhs, const edge& rhs) { return lhs.id == rhs.id; }
    friend bool operator!=(const edge& lhs, const edge& rhs) { return lhs.id != rhs.id; }
};


} // namespace graph_impl

} // namespace internal

template<class C = ll>
struct graph : std::vector<std::vector<internal::graph_impl::edge<C,internal::size_t>>> {
    using size_type = internal::size_t;
    using cost_type = C;

    using edge = typename internal::graph_impl::edge<cost_type,size_type>;

    enum class edge_type { undirected, directed };

  private:
    size_type _directed_edge_count, _undirected_edge_count;

  protected:
    inline void _add_edge(const size_type u, const size_type v, const cost_type w) {
        this->operator[](u).emplace_back(u, v, w);
        ++this->_directed_edge_count;
    }

  public:
    explicit graph(const size_type n = 0) : std::vector<std::vector<edge>>(n) {}

    inline void clear() { ITR(row, *this) row.clear(); }

    // using std::vector<std::vector<edge>>::size;

    // using std::vector<std::vector<edge>>::begin;
    // using std::vector<std::vector<edge>>::cbegin;
    // using std::vector<std::vector<edge>>::end;
    // using std::vector<std::vector<edge>>::cend;

    inline size_type vertexes() const { return this->size(); }
    inline size_type edges() const { return this->inputted.size(); }

    template<const edge_type EDGE_TYPE = edge_type::directed>
    inline void add_edge(const size_type u, const size_type v, const cost_type w = 1) {
        assert(0 <= u and u < this->vertexes()), assert(0 <= v and v < this->vertexes());
        this->_add_edge(u, v, w);
        if constexpr(EDGE_TYPE == edge_type::undirected) this->_add_edge(v, u, w);
        ++this->_undirected_edge_count;
    }

    inline void add_edge_bidirectionally(const size_type u, const size_type v, const cost_type w = 1) {
        this->add_edge<edge_type::undirected>(u, v, w);
    }

    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, const edge_type EDGE_TYPE = edge_type::directed, class Stream = std::istream>
    void inline read(const size_type edges, Stream *const ist = &std::cin) {
        REP(edges) {
            size_type u, v; cost_type w = 1; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<EDGE_TYPE>(u, v, w);
        }
    }
    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, class Stream = std::istream>
    void inline read_bidirectionally(const size_type edges, Stream *const ist = &std::cin) {
        REP(edges) {
            size_type u, v; cost_type w = 1; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<edge_type::undirected>(u, v, w);
        }
    }

    // graph/shortest_path.hpp
    template<class cost_t = cost_type> inline void distances_without_cost(const size_type, std::vector<cost_t> *const) const;
    template<class cost_t = cost_type> inline std::vector<cost_t> distances_without_cost(const size_type) const;

    // graph/dijkstra.hpp
    template<class cost_t = cost_type> inline void distances_with_01cost(const size_type, std::vector<cost_t> *const) const;
    template<class cost_t = cost_type> inline std::vector<cost_t> distances_with_01cost(const size_type) const;

    // graph/dijkstra.hpp
    template<class cost_t = cost_type> inline void distances(const size_type, std::vector<cost_t> *const) const;
    template<class cost_t = cost_type> inline std::vector<cost_t> distances(const size_type) const;

    // graph/topological_sort.hpp
    inline bool sort_topologically(std::vector<size_type> *const ) const;
    inline bool sort_topologically() const;

    // graph/topological_sort.hpp
    template<class> inline bool sort_topologically_with_priority(std::vector<size_type> *const) const;
    template<class> inline bool sort_topologically_with_priority() const;

    // graph/minimum_paph_cover.hpp
    inline size_type minimum_paph_cover_size_as_dag() const;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = cost_type>
    inline cost_t minimum_spanning_tree_cost() const;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = cost_type>
    inline cost_t maximum_spanning_tree_cost() const;

    // graph/connected_components.hpp
    inline dsu components() const;

    // graph/from_grid.hpp
    template<class G, class U = char>
    inline void from_grid(const G&, U = '.');

    // graph/manhattan_minimum_spanning_tree.hpp
    template<class I, class J = I, class distance_type = cost_type, class = internal::size_t>
    inline distance_type build_manhattan_mst(const I, const I, const J, const J);
};

} // namespace lib

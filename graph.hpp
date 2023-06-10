#pragma once


#include <cassert>
#include <tuple>
#include <vector>
#include <iostream>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "data_structure/disjoint_set_union.hpp"


namespace lib {

namespace internal {

namespace graph_impl {


template<class cost_t, class size_type> struct edge {
  private:
    inline static internal::size_t unique() noexcept(NO_EXCEPT) { static internal::size_t id = 0; return id++; }

  public:
    using cost_type = cost_t;

    const internal::size_t id = unique();
    const size_type from, to; const cost_t cost;

    edge(const size_type u, const size_type v, const cost_t w) noexcept(NO_EXCEPT) : from(u), to(v), cost(w) {}

    operator size_type() const noexcept(NO_EXCEPT) { return this->to; }

    inline size_type opposite(const size_type v) const noexcept(NO_EXCEPT) {
        if(this->from == v) return this->to;
        if(this->to == v) return this->from;
        assert(false);
    }

    std::tuple<size_type,size_type,cost_t> _debug() const noexcept(NO_EXCEPT) { return { from, to, cost }; };

    friend bool operator==(const edge& lhs, const edge& rhs) noexcept(NO_EXCEPT) { return lhs.id == rhs.id; }
    friend bool operator!=(const edge& lhs, const edge& rhs) noexcept(NO_EXCEPT) { return lhs.id != rhs.id; }
};


} // namespace graph_impl

} // namespace internal

template<class C = ll>
struct graph : std::vector<std::vector<internal::graph_impl::edge<C,internal::size_t>>> {
    using size_type = internal::size_t;
    using cost_type = C;

    using edge_type = typename internal::graph_impl::edge<cost_type,size_type>;

    enum class edge_kind { undirected, directed };

  private:
    size_type _directed_edge_count = 0, _undirected_edge_count = 0;

    std::vector<edge_type> _edges;
    std::vector<size_type> _out_degs, _in_degs;

  protected:
    inline void _add_edge(const size_type u, const size_type v, const cost_type w) noexcept(NO_EXCEPT) {
        this->operator[](u).emplace_back(u, v, w);
        ++_out_degs[u], ++_in_degs[v];
        ++this->_directed_edge_count;
    }

  public:
    explicit graph(const size_type n = 0) noexcept(NO_EXCEPT)
      : std::vector<std::vector<edge_type>>(n), _out_degs(n), _in_degs(n)
    {}

    inline void clear() noexcept(NO_EXCEPT) { this->std::vector<std::vector<edge_type>>::clear(); }

    // using std::vector<std::vector<edge_type>>::size;

    // using std::vector<std::vector<edge_type>>::begin;
    // using std::vector<std::vector<edge_type>>::cbegin;
    // using std::vector<std::vector<edge_type>>::end;
    // using std::vector<std::vector<edge_type>>::cend;

    inline const auto& edges() const noexcept(NO_EXCEPT) { return this->_edges; }
    inline const auto& edge(const size_type k) const noexcept(NO_EXCEPT) { return this->_edges[k]; }

    inline const auto& degrees() const noexcept(NO_EXCEPT) { return this->_in_degs; }
    inline const auto& degree(const size_type k) const noexcept(NO_EXCEPT) { return this->_in_degs[k]; }

    inline const auto& in_degrees() const noexcept(NO_EXCEPT) { return this->_in_degs; }
    inline const auto& in_degree(const size_type k) const noexcept(NO_EXCEPT) { return this->_in_degs[k]; }

    inline const auto& out_degrees() const noexcept(NO_EXCEPT) { return this->_out_degs; }
    inline const auto& out_degree(const size_type k) const noexcept(NO_EXCEPT) { return this->_out_degs[k]; }

    inline size_type vertices() const noexcept(NO_EXCEPT) { return static_cast<size_type>(this->size()); }

    inline size_type directed_edges_count() const noexcept(NO_EXCEPT) { return this->_directed_edge_count; }

    template<const edge_kind EDGE_TYPE = edge_kind::directed>
    inline void add_edge(const size_type u, const size_type v, const cost_type w = 1) noexcept(NO_EXCEPT) {
        assert(0 <= u and u < this->vertices()), assert(0 <= v and v < this->vertices());
        this->_edges.emplace_back(u, v, w);
        this->_add_edge(u, v, w);
        if constexpr(EDGE_TYPE == edge_kind::undirected) this->_add_edge(v, u, w);
    }

    inline void add_edge_bidirectionally(const size_type u, const size_type v, const cost_type w = 1) noexcept(NO_EXCEPT) {
        this->add_edge<edge_kind::undirected>(u, v, w);
    }

    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, const edge_kind EDGE_TYPE = edge_kind::directed, class Stream = std::istream>
    void inline read(const size_type edges, Stream *const ist = &std::cin) noexcept(NO_EXCEPT) {
        REP(edges) {
            size_type u, v; cost_type w = 1; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<EDGE_TYPE>(u, v, w);
        }
    }
    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, class Stream = std::istream>
    void inline read_bidirectionally(const size_type edges, Stream *const ist = &std::cin) noexcept(NO_EXCEPT) {
        REP(edges) {
            size_type u, v; cost_type w = 1; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<edge_kind::undirected>(u, v, w);
        }
    }

    // graph/shortest_path.hpp
    template<class cost_t = cost_type> inline void distances_without_cost(const size_type, std::vector<cost_t> *const) const noexcept(NO_EXCEPT) ;
    template<class cost_t = cost_type> inline std::vector<cost_t> distances_without_cost(const size_type) const noexcept(NO_EXCEPT) ;

    // graph/dijkstra.hpp
    template<class cost_t = cost_type> inline void distances_with_01cost(const size_type, std::vector<cost_t> *const) const noexcept(NO_EXCEPT) ;
    template<class cost_t = cost_type> inline std::vector<cost_t> distances_with_01cost(const size_type) const noexcept(NO_EXCEPT) ;

    // graph/dijkstra.hpp
    template<class cost_t = cost_type> inline void distances(const size_type, std::vector<cost_t> *const) const noexcept(NO_EXCEPT) ;
    template<class cost_t = cost_type> inline std::vector<cost_t> distances(const size_type) const noexcept(NO_EXCEPT) ;

    // graph/topological_sort.hpp
    inline bool sort_topologically(std::vector<size_type> *const ) const noexcept(NO_EXCEPT) ;
    inline bool sort_topologically() const noexcept(NO_EXCEPT) ;

    // graph/topological_sort.hpp
    template<class> inline bool sort_topologically_with_priority(std::vector<size_type> *const) const noexcept(NO_EXCEPT) ;
    template<class> inline bool sort_topologically_with_priority() const noexcept(NO_EXCEPT) ;

    // graph/minimum_paph_cover.hpp
    inline size_type minimum_paph_cover_size_as_dag() const noexcept(NO_EXCEPT) ;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = cost_type>
    inline cost_t minimum_spanning_tree(graph *const = nullptr) const noexcept(NO_EXCEPT) ;

    // graph/spanning_tree_cost.hpp
    template<class cost_t = cost_type>
    inline cost_t maximum_spanning_tree(graph *const = nullptr) const noexcept(NO_EXCEPT) ;

    // graph/connected_components.hpp
    inline dsu components() const noexcept(NO_EXCEPT) ;

    // graph/from_grid.hpp
    template<bool = false, class G, class U = char>
    inline void from_grid(const G&, U = '.') noexcept(NO_EXCEPT) ;

    // graph/manhattan_minimum_spanning_tree.hpp
    template<class I, class J = I, class distance_type = cost_type, class = internal::size_t>
    inline distance_type build_manhattan_mst(const I, const I, const J, const J) noexcept(NO_EXCEPT) ;
};

} // namespace lib

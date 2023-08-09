#pragma once


#include <cassert>
#include <tuple>
#include <iostream>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "utility/functional.hpp"
#include "adapter/valarray.hpp"
#include "adapter/vector.hpp"
#include "adapter/io.hpp"

#include "grid.hpp"
#include "data_structure/disjoint_set_union.hpp"

#include "numeric/virtual_map.hpp"

#include "auto_holder.hpp"


namespace lib {

namespace internal {

namespace graph_impl {


template<class Node,class Cost> struct edge {
  private:
    inline static internal::size_t unique() noexcept(NO_EXCEPT) { static internal::size_t id = 0; return id++; }

  public:
    using cost_type = Cost;
    using node_type = Node;

    const internal::size_t id = unique();
    const node_type from, to; const Cost cost;

    edge(const node_type u, const node_type v, const Cost w = 1) noexcept(NO_EXCEPT) : from(u), to(v), cost(w) {}

    operator node_type() const noexcept(NO_EXCEPT) { return this->to; }

    inline node_type opposite(const node_type v) const noexcept(NO_EXCEPT) {
        if(this->from == v) return this->to;
        if(this->to == v) return this->from;
        assert(false);
    }

    std::tuple<node_type,node_type,Cost> _debug() const noexcept(NO_EXCEPT) { return { from, to, cost }; };

    friend bool operator==(const edge& lhs, const edge& rhs) noexcept(NO_EXCEPT) { return lhs.id == rhs.id; }
    friend bool operator!=(const edge& lhs, const edge& rhs) noexcept(NO_EXCEPT) { return lhs.id != rhs.id; }
};


template<class NodeType, class CostType, class EdgeCollector>
struct regular_base : EdgeCollector {
    using EdgeCollector::EdgeCollector;
    using size_type = size_t;
    using node_type = NodeType;
    using cost_type = CostType;

    inline size_type size() const noexcept(NO_EXCEPT) { return static_cast<size_type>(this->EdgeCollector::size()); }
};

template<class NodeType, class CostType, class Map>
struct virtual_base : Map {
  public:
    using size_type = internal::size_t;
    using node_type = NodeType;
    using cost_type = CostType;

  protected:
    size_type _n = 0;

  public:
    template<class F>
    explicit virtual_base(const size_type n, const F f) noexcept(NO_EXCEPT) : Map(f), _n(n) {}

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
};


template<class NodeType, class CostType, template<class...> class Container>
struct regular_core : regular_base<NodeType,CostType,Container<vector<internal::graph_impl::edge<NodeType,CostType>>>> {
    using size_type = internal::size_t;
    using node_type = NodeType;
    using cost_type = CostType;

    using edge_type = typename internal::graph_impl::edge<node_type,cost_type>;

    enum class edge_kind { undirected, directed };

  private:
    using base = regular_base<NodeType,CostType,Container<vector<edge_type>>>;

    size_type _directed_edge_count = 0, _undirected_edge_count = 0;

    Container<edge_type> _edges;
    Container<size_type> _out_degs, _in_degs;

  protected:
    inline void _add_edge(const size_type u, const size_type v, const cost_type w) noexcept(NO_EXCEPT) {
        this->operator[](u).emplace_back(u, v, w);
        ++_out_degs[u], ++_in_degs[v];
        ++this->_directed_edge_count;
    }

  public:
    explicit regular_core() noexcept(NO_EXCEPT) : base() {}
    explicit regular_core(const size_type n = 0) noexcept(NO_EXCEPT)
      : base(n), _out_degs(n), _in_degs(n)
    {}

    inline auto& clear() noexcept(NO_EXCEPT) {
        this->base::clear(), this->_out_degs.clear(), this->_in_degs.clear();
        return *this;
    }

    inline auto& resize(const size_type n) noexcept(NO_EXCEPT) {
        this->base::resize(n), this->_out_degs.resize(n), this->_in_degs.resize(n);
        return *this;
    }

    inline const auto& edges() const noexcept(NO_EXCEPT) { return this->_edges; }
    inline const auto& edge(const size_type k) const noexcept(NO_EXCEPT) { return this->_edges[k]; }

    inline const auto& degrees() const noexcept(NO_EXCEPT) { return this->_in_degs; }
    inline const auto& degree(const size_type k) const noexcept(NO_EXCEPT) { return this->_in_degs[k]; }

    inline const auto& in_degrees() const noexcept(NO_EXCEPT) { return this->_in_degs; }
    inline const auto& in_degree(const size_type k) const noexcept(NO_EXCEPT) { return this->_in_degs[k]; }

    inline const auto& out_degrees() const noexcept(NO_EXCEPT) { return this->_out_degs; }
    inline const auto& out_degree(const size_type k) const noexcept(NO_EXCEPT) { return this->_out_degs[k]; }

    inline size_type directed_edges_count() const noexcept(NO_EXCEPT) { return this->_directed_edge_count; }

    template<class R = valgrid<bool>>
    inline auto make_has_edges() const noexcept(NO_EXCEPT) {
        R res(this->size(), this->size(), false);
        REP(i, this->size()) ITR(j, this->operator[](i)) res[i][j] = true;
        return res;
    }

    template<const bool SELF_ZERO = true, class T = cost_type, class R = valgrid<T>>
    inline auto make_initial_distance_matrix() const noexcept(NO_EXCEPT) {
        R res(this->size(), this->size(), numeric_limits<T>::arithmetic_infinity());
        if constexpr(SELF_ZERO) REP(i, this->size()) res[i][i] = 0;
        REP(i, this->size()) ITR(j, this->operator[](i)) res[i][j] = j.cost;
        return res;
    }

    template<const bool SELF_ZERO = true, class T = cost_type, class R = valgrid<T>>
    inline auto make_distance_matrix() const noexcept(NO_EXCEPT) {
        R res = this->make_initial_distance_matrix<SELF_ZERO,T,R>();
        REP(k, this->size()) REP(i, this->size()) REP(j, this->size()) {
            chmin(res[i][j], res[i][k] + res[k][j]);
        }
        return res;
    }

    template<const edge_kind EDGE_TYPE = edge_kind::directed>
    inline void add_edge(const node_type u, const node_type v, const cost_type w = 1) noexcept(NO_EXCEPT) {
        assert(0 <= u and u < this->size()), assert(0 <= v and v < this->size());
        this->_edges.emplace_back(u, v, w);
        this->_add_edge(u, v, w);
        if constexpr(EDGE_TYPE == edge_kind::undirected) this->_add_edge(v, u, w);
    }

    inline void add_edge_bidirectionally(const node_type u, const node_type v, const cost_type w = 1) noexcept(NO_EXCEPT) {
        this->add_edge<edge_kind::undirected>(u, v, w);
    }

    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, const edge_kind EDGE_TYPE = edge_kind::directed, class Stream = input_adapter<>>
    void inline read(const size_type edges, Stream *const ist = &_input) noexcept(NO_EXCEPT) {
        REP(edges) {
            node_type u, v; cost_type w = 1; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<EDGE_TYPE>(u, v, w);
        }
    }
    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, class Stream = input_adapter<>>
    void inline read_bidirectionally(const size_type edges, Stream *const ist = &_input) noexcept(NO_EXCEPT) {
        REP(edges) {
            node_type u, v; cost_type w = 1; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<edge_kind::undirected>(u, v, w);
        }
    }
};

template<class Graph>
struct mixin : Graph {
    using Graph::Graph;
    using size_type = typename Graph::size_type;
    using node_type = typename Graph::node_type;
    using cost_type = typename Graph::cost_type;

    inline size_type vertices() const noexcept(NO_EXCEPT) { return static_cast<size_type>(this->Graph::size()); }


  public:
    // graph/shortest_path.hpp
    template<class Cost = cost_type, class Dist, class Prev = std::nullptr_t>
    inline void distances_without_cost(const node_type&, Dist *const, Prev *const = nullptr, const node_type& = {}, const node_type& = {}) const noexcept(NO_EXCEPT) ;

    template<class Cost = cost_type>
    inline auto_holder<node_type,Cost> distances_without_cost(const node_type) const noexcept(NO_EXCEPT) ;

    // graph/dijkstra.hpp
    template<class Cost = cost_type, class Dist, class Prev = std::nullptr_t>
    inline void distances_with_01cost(const node_type&, Dist *const, Prev *const = nullptr, const node_type& = {}, const node_type& = {}) const noexcept(NO_EXCEPT) ;

    template<class Cost = cost_type>
    inline auto_holder<node_type,Cost> distances_with_01cost(const node_type) const noexcept(NO_EXCEPT) ;

    // graph/dijkstra.hpp
    template<class Cost = cost_type, class Dist, class Prev = std::nullptr_t>
    inline void distances_with_cost(const node_type&, Dist *const, Prev *const = nullptr, const node_type& = {}, const node_type& = {}) const noexcept(NO_EXCEPT) ;

    template<class Cost = cost_type>
    inline auto_holder<node_type,Cost> distances_with_cost(const node_type) const noexcept(NO_EXCEPT) ;

    // graph/topological_sort.hpp
    inline bool sort_topologically(vector<node_type> *const ) const noexcept(NO_EXCEPT) ;
    inline bool sort_topologically() const noexcept(NO_EXCEPT) ;

    // graph/topological_sort.hpp
    template<class> inline bool sort_topologically_with_priority(vector<node_type> *const) const noexcept(NO_EXCEPT) ;
    template<class> inline bool sort_topologically_with_priority() const noexcept(NO_EXCEPT) ;

    // graph/minimum_paph_cover.hpp
    inline size_type minimum_paph_cover_size_as_dag() const noexcept(NO_EXCEPT) ;

    // graph/spanning_tree_cost.hpp
    template<class Cost = cost_type>
    inline Cost minimum_spanning_tree(mixin *const = nullptr) const noexcept(NO_EXCEPT) ;

    // graph/spanning_tree_cost.hpp
    template<class Cost = cost_type>
    inline Cost maximum_spanning_tree(mixin *const = nullptr) const noexcept(NO_EXCEPT) ;

    // graph/connected_components.hpp
    inline dsu components() const noexcept(NO_EXCEPT) ;

    // graph/from_grid.hpp
    template<bool = false, class G, class U = char>
    inline void from_grid(const G&, U = '.') noexcept(NO_EXCEPT) ;

    // graph/manhattan_minimum_spanning_tree.hpp
    template<class I, class J = I, class distance_type = cost_type, class = internal::size_t>
    inline distance_type build_manhattan_mst(const I, const I, const J, const J) noexcept(NO_EXCEPT) ;
};

} // namespace graph_impl

} // namespace internal

template<class Cost = std::int64_t, class Node = internal::size_t, template<class...> class Container = vector>
struct graph : internal::graph_impl::mixin<internal::graph_impl::regular_core<Node,Cost,Container>> {
  private:
    using base = internal::graph_impl::mixin<internal::graph_impl::regular_core<Node,Cost,Container>>;

  public:
    using size_type = typename base::size_type;
    using node_type = typename base::node_type;
    using edge = typename internal::graph_impl::edge<node_type,Cost>;

    explicit graph(const size_type n = 0) noexcept(NO_EXCEPT) : base(n) {}
};

template<class Node = internal::size_t, class Cost = std::int64_t, class Edges = virtual_map<Node,vector<typename internal::graph_impl::edge<Node,Cost>>>>
struct virtual_graph : internal::graph_impl::mixin<internal::graph_impl::virtual_base<Node,Cost,Edges>> {
  private:
    using base = internal::graph_impl::mixin<internal::graph_impl::virtual_base<Node,Cost,Edges>>;

  public:
    using size_type = typename base::size_type;
    using edge = typename internal::graph_impl::edge<Node,Cost>;

  private:
    size_type _n = 0;

  public:
    template<class F>
    explicit virtual_graph(const F f, const size_type n = 0) noexcept(NO_EXCEPT) : base(n, f), _n(n) {}
};


} // namespace lib

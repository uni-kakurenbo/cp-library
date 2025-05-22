#pragma once


#include <cassert>
#include <tuple>
#include <iostream>
#include <ranges>


#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"
#include "internal/auto_holder.hpp"

#include "utility/functional.hpp"

#include "graph/internal/concepts.hpp"

#include "adaptor/valarray.hpp"
#include "adaptor/vector.hpp"
#include "adaptor/io.hpp"

#include "structure/grid.hpp"
#include "data_structure/disjoint_set.hpp"

#include "adaptor/virtual_map.hpp"



namespace uni {

namespace internal {

namespace graph_impl {


template<class Node,class Cost> struct edge {
  private:
    inline static internal::size_t unique() noexcept(NO_EXCEPT) { static internal::size_t id = 0; return id++; }

  public:
    using cost_type = Cost;
    using node_type = Node;
    using size_type = internal::size_t;

    const size_type id = unique();
    const node_type from, to; const Cost cost;
    const size_type index = 0;

    edge(const node_type u, const node_type v, const Cost w = 1, const size_type i = 0) noexcept(NO_EXCEPT)
      : from(u), to(v), cost(w), index(i)
    {}

    operator node_type() const noexcept(NO_EXCEPT) { return this->to; }

    inline node_type opposite(const node_type v) const noexcept(NO_EXCEPT) {
        if(this->from == v) return this->to;
        if(this->to == v) return this->from;
        assert(false);
    }

    auto _debug() const { return std::make_tuple(index, from, to, cost); };

    friend bool operator==(const edge& lhs, const edge& rhs) noexcept(NO_EXCEPT) { return lhs.id == rhs.id; }
    friend bool operator!=(const edge& lhs, const edge& rhs) noexcept(NO_EXCEPT) { return lhs.id != rhs.id; }
};


template<class NodeType, class CostType, class EdgeCollector>
struct regular_base : EdgeCollector {
    using EdgeCollector::EdgeCollector;
    using size_type = internal::size_t;
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
    explicit virtual_base(const size_type n, F&& f) noexcept(NO_EXCEPT)
      : Map(std::forward<F>(f)), _n(n)
    {}

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
    inline void _add_edge(const size_type u, const size_type v, const cost_type w, const size_type k) noexcept(NO_EXCEPT) {
        this->operator[](u).emplace_back(u, v, w, k);
        ++_out_degs[u], ++_in_degs[v];
        ++this->_directed_edge_count;
    }

  public:
    explicit regular_core() noexcept(NO_EXCEPT) : base() {}
    explicit regular_core(const size_type n = 0) noexcept(NO_EXCEPT)
      : base(n), _out_degs(n), _in_degs(n)
    {}

    auto& clear() noexcept(NO_EXCEPT) {
        this->_directed_edge_count = 0, this->_undirected_edge_count = 0;
        this->base::clear(), this->_edges.clear();
        this->_out_degs.clear(), this->_in_degs.clear();
        return *this;
    }

    auto& resize(const size_type n) noexcept(NO_EXCEPT) {
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
    auto make_has_edges() const noexcept(NO_EXCEPT) {
        R res(this->size(), this->size(), false);
        REP(i, this->size()) ITR(j, this->operator[](i)) res[i][j] = true;
        return res;
    }

    template<bool SELF_ZERO = true, class T = cost_type, class R = valgrid<T>>
    auto make_initial_distance_matrix() const noexcept(NO_EXCEPT) {
        R res(this->size(), this->size(), numeric_limits<T>::arithmetic_infinity());
        if constexpr(SELF_ZERO) REP(i, this->size()) res[i][i] = 0;
        REP(i, this->size()) ITR(j, this->operator[](i)) res[i][j] = j.cost;
        return res;
    }

    template<bool SELF_ZERO = true, class T = cost_type, class R = valgrid<T>>
    auto make_distance_matrix() const noexcept(NO_EXCEPT) {
        R res = this->make_initial_distance_matrix<SELF_ZERO,T,R>();
        REP(k, this->size()) REP(i, this->size()) REP(j, this->size()) {
            chmin(res[i][j], res[i][k] + res[k][j]);
        }
        return res;
    }

    template<edge_kind EDGE_TYPE = edge_kind::directed>
    auto add_edge(const node_type u, const node_type v, const cost_type w = 1) noexcept(NO_EXCEPT) {
        assert(0 <= u and u < this->size()), assert(0 <= v and v < this->size());
        const size_type k = this->edges().size();
        this->_edges.emplace_back(u, v, w, k);
        this->_add_edge(u, v, w, k);
        if constexpr(EDGE_TYPE == edge_kind::undirected) this->_add_edge(v, u, w, k);
        return k;
    }

    inline auto add_edge_bidirectionally(const node_type u, const node_type v, const cost_type w = 1) noexcept(NO_EXCEPT) {
        return this->add_edge<edge_kind::undirected>(u, v, w);
    }

    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, const edge_kind EDGE_TYPE = edge_kind::directed, class Stream = input_adaptor<>>
    void read(const size_type edges, Stream *const ist = &_input) noexcept(NO_EXCEPT) {
        REP(edges) {
            node_type u, v; cost_type w = 1; *ist >> u >> v; if(ONE_ORIGIN) --u, --v;
            if(WEIGHTED) *ist >> w;
            this->add_edge<EDGE_TYPE>(u, v, w);
        }
    }

    template<bool WEIGHTED = false, bool ONE_ORIGIN = true, class Stream = input_adaptor<>>
    void read_bidirectionally(const size_type edges, Stream *const ist = &_input) noexcept(NO_EXCEPT) {
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
    template<item_or_convertible_range<node_type> Source, class Dist, class Prev = std::nullptr_t>
    void shortest_path_without_cost(Source&&, Dist *const, Prev *const = nullptr, const node_type& = -1, const node_type& = -2) const noexcept(NO_EXCEPT);

    template<item_or_convertible_range<node_type> Source>
    auto shortest_path_without_cost(Source&&) const noexcept(NO_EXCEPT);

    // graph/dijkstra.hpp
    template<item_or_convertible_range<node_type> Source, class Dist, class Prev = std::nullptr_t>
    void shortest_path_with_01cost(Source&&, Dist *const, Prev *const = nullptr, const node_type& = -1, const node_type& = -2) const noexcept(NO_EXCEPT);

    template<item_or_convertible_range<node_type> Source>
    auto shortest_path_with_01cost(Source&&) const noexcept(NO_EXCEPT);

    // graph/dijkstra.hpp
    template<item_or_convertible_range<node_type> Source, class Dist, class Prev = std::nullptr_t>
    void shortest_path_with_cost(Source&&, Dist *const, Prev *const = nullptr, const node_type& = -1, const node_type& = -2) const noexcept(NO_EXCEPT);

    template<item_or_convertible_range<node_type> Source>
    auto shortest_path_with_cost(Source&&) const noexcept(NO_EXCEPT);

    // graph/topological_sort.hpp
    template<internal::topological_sortable_with<Graph> Range>
    bool sort_topologically(Range *const) const noexcept(NO_EXCEPT);
    bool sort_topologically() const noexcept(NO_EXCEPT);

    // graph/topological_sort.hpp
    template<class> bool sort_topologically_with_priority(vector<node_type> *const) const noexcept(NO_EXCEPT);
    template<class> bool sort_topologically_with_priority() const noexcept(NO_EXCEPT);

    // graph/minimum_paph_cover.hpp
    size_type minimum_paph_cover_size_as_dag() const noexcept(NO_EXCEPT);

    // graph/spanning_tree_cost.hpp
    auto minimum_spanning_tree(mixin *const = nullptr) const noexcept(NO_EXCEPT);

    // graph/spanning_tree_cost.hpp
    auto maximum_spanning_tree(mixin *const = nullptr) const noexcept(NO_EXCEPT);

    // graph/reachability.hpp
    template<std::ranges::sized_range R>
    auto test_reachability(R&&) const noexcept(NO_EXCEPT);

    // graph/connected_components.hpp
    disjoint_set components() const noexcept(NO_EXCEPT);

    // graph/connected_components.hpp
    bool is_bipartite() const noexcept(NO_EXCEPT);

    // graph/connected_components.hpp
    template<class Colors>
    bool is_bipartite(Colors *const) const noexcept(NO_EXCEPT);

    // graph/parse_grid.hpp
    template<bool = false, class G, class U = char>
    void parse_grid(const G&, U = '.') noexcept(NO_EXCEPT);

    // graph/manhattan_minimum_spanning_tree.hpp
    template<
        std::input_iterator I0, std::input_iterator I1,
        std::sentinel_for<I0> S0, std::sentinel_for<I1> S1
    >
    cost_type build_manhattan_mst(I0, S0, I1, S1) noexcept(NO_EXCEPT);
};


} // namespace graph_impl

} // namespace internal


template<class Cost = std::int64_t, class Node = internal::size_t, template<class...> class Container = vector>
struct graph : internal::graph_impl::mixin<internal::graph_impl::regular_core<Node, Cost, Container>> {
  private:
    using base = internal::graph_impl::mixin<internal::graph_impl::regular_core<Node, Cost, Container>>;

  public:
    using size_type = typename base::size_type;
    using node_type = typename base::node_type;
    using edge = typename internal::graph_impl::edge<node_type,Cost>;

    explicit graph(const size_type n = 0) noexcept(NO_EXCEPT) : base(n) {}
};

template<class Node = internal::size_t, class Cost = std::int64_t, class Edges = virtual_map<Node, vector<typename internal::graph_impl::edge<Node, Cost>>>>
struct virtual_graph : internal::graph_impl::mixin<internal::graph_impl::virtual_base<Node, Cost, Edges>> {
  private:
    using base = internal::graph_impl::mixin<internal::graph_impl::virtual_base<Node, Cost, Edges>>;

  public:
    using size_type = typename base::size_type;
    using edge = typename internal::graph_impl::edge<Node, Cost>;

  private:
    size_type _n = 0;

  public:
    template<class F>
    explicit virtual_graph(F&& f, const size_type n = 0) noexcept(NO_EXCEPT)
      : base(n, std::forward<F>(f)), _n(n)
    {}
};


} // namespace uni

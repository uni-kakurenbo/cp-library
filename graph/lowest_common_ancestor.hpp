#pragma once


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/bit.hpp"
#include "adapter/valarray.hpp"

#include "structure/graph.hpp"


namespace lib {


template<class Graph>
struct lowest_common_ancestor {
    using size_type = internal::size_t;
    using graph_type = Graph;
    using edge_type = typename graph_type::edge_type;
    using cost_type = typename graph_type::cost_type;

    valarray<valarray<size_type>> parent;
    valarray<size_type> depth;
    valarray<cost_type> cost;

  private:
    void dfs(const graph_type &G, const edge_type& e) noexcept(NO_EXCEPT) {
        this->parent[0][e.to] = e.from;
        if(e.from >= 0) {
            this->depth[e.to] = this->depth[e.from] + 1;
            this->cost[e.to] = this->cost[e.from] + e.cost;
        }
        ITR(f, G[e.to]) {
            if(f.to != e.from) dfs(G, f);
        }
    }

  public:
    lowest_common_ancestor(const graph_type &G, const size_type root = 0) noexcept(NO_EXCEPT) { this->init(G, root); }

    void init(const graph_type &G, const size_type root = 0) noexcept(NO_EXCEPT) {
        const size_type n = static_cast<size_type>(G.size());
        const size_type d = lib::bit_width<std::make_unsigned_t<size_type>>(n);

        this->parent.assign(d, valarray<size_type>(n, -1));
        this->depth.assign(n, 0), this->cost.assign(n, 0);

        this->dfs(G, edge_type(-1, root, 0));

        REP(k, d-1) REP(v, n) {
            if(this->parent[k][v] < 0) this->parent[k+1][v] = -1;
            else this->parent[k+1][v] = this->parent[k][this->parent[k][v]];
        }
    }

    size_type operator()(const size_type u, const size_type v) const noexcept(NO_EXCEPT) {
        return this->find(u, v);
    }

    size_type find(size_type u, size_type v) const noexcept(NO_EXCEPT) {
        if(this->depth[u] < this->depth[v]) std::swap(u, v);
        size_type d = static_cast<size_type>(this->parent.size());

        REP(k, d) {
            if((this->depth[u] - this->depth[v]) >> k & 1) u = this->parent[k][u];
        }

        if(u == v) return u;

        REPD(k, d) {
            if(this->parent[k][u] != this->parent[k][v]) {
                u = this->parent[k][u];
                v = this->parent[k][v];
            }
        }

        return this->parent[0][u];
    }

    size_type distance_ignored_cost(const size_type u, const size_type v) const noexcept(NO_EXCEPT) {
        return this->depth[u] + this->depth[v] - 2 * this->depth[find(u, v)];
    }

    size_type distance(const size_type u, const size_type v) const noexcept(NO_EXCEPT) {
        return this->cost[u] + this->cost[v] - 2 * this->cost[find(u, v)];
    }
};


} // namespace lib

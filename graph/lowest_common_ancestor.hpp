#pragma once

#include <vector>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "graph.hpp"
#include "numeric/bit.hpp"

namespace lib {


struct lowest_common_ancestor {
    using size_type = internal::size_t;
    std::vector<std::vector<size_type>> parent;
    std::vector<size_type> dists;

  private:
    template<class graph>
    void dfs(const graph &G, const size_type v, const size_type p, const size_type d) noexcept(NO_EXCEPT) {
        this->parent[0][v] = p;
        this->dists[v] = d;
        ITR(e, G[v]) {
            if(e.to != p) dfs(G, e.to, v, d+1);
        }
    }

  public:
    template<class graph>
    lowest_common_ancestor(const graph &G, const size_type root = 0) noexcept(NO_EXCEPT) { this->init(G, root); }

    template<class graph>
    void init(const graph &G, const size_type root = 0) noexcept(NO_EXCEPT) {
        const size_type V = static_cast<size_type>(G.size());
        const size_type K = lib::bit_width<std::make_unsigned_t<size_type>>(V);

        this->parent.assign(K, std::vector<size_type>(V, -1));
        this->dists.assign(V, -1);

        this->dfs(G, root, -1, 0);

        REP(k, K-1) REP(v, V) {
            if(this->parent[k][v] < 0) this->parent[k+1][v] = -1;
            else this->parent[k+1][v] = this->parent[k][this->parent[k][v]];
        }
    }

    size_type operator()(const size_type u, const size_type v) const noexcept(NO_EXCEPT) {
        return this->find(u, v);
    }

    size_type find(size_type u, size_type v) const noexcept(NO_EXCEPT) {
        if(this->dists[u] < this->dists[v]) std::swap(u, v);
        size_type K = static_cast<size_type>(this->parent.size());

        REP(k, K) {
            if((this->dists[u] - this->dists[v]) >> k & 1) u = this->parent[k][u];
        }

        if(u == v) return u;

        REPD(k, K) {
            if(this->parent[k][u] != this->parent[k][v]) {
                u = this->parent[k][u];
                v = this->parent[k][v];
            }
        }

        return this->parent[0][u];
    }

    size_type distance(const size_type u, const size_type v) const noexcept(NO_EXCEPT) {
        return this->dists[u] + this->dists[v] - 2 * this->dists[find(u, v)];
    }
};


} // namespace lib

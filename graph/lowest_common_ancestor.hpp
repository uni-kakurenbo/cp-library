#pragma once

#include <vector>

#include "snippet/iterations.hpp"
#include "internal/types.hpp"

#include "graph.hpp"

namespace lib {


struct lowest_common_ancestor {
    using size_type = internal::size_t;
    std::vector<std::vector<size_type>> parent;
    std::vector<size_type> dists;

  private:
    template<class graph>
    void dfs(const graph &G, const size_type v, const size_type p, const size_type d) {
        parent[0][v] = p;
        dists[v] = d;
        ITR(e, G[v]) {
            if(e.to != p) dfs(G, e.to, v, d+1);
        }
    }

  public:
    template<class graph>
    lowest_common_ancestor(const graph &G, const size_type root = 0) { this->init(G, root); }

    template<class graph>
    void init(const graph &G, const size_type root = 0) {
        const size_type V = G.size();
        size_type K = 1; while((1 << ++K) < V);

        parent.assign(K, std::vector<size_type>(V, -1));
        dists.assign(V, -1);

        this->dfs(G, root, -1, 0);

        REP(k, K-1) REP(v, V) {
            if(parent[k][v] < 0) parent[k+1][v] = -1;
            else parent[k+1][v] = parent[k][parent[k][v]];
        }
    }

    size_type operator()(const size_type u, const size_type v) const {
        return this->find(u, v);
    }

    size_type find(size_type u, size_type v) const {
        if(dists[u] < dists[v]) std::swap(u, v);
        size_type K = parent.size();

        REP(k, K) {
            if((dists[u] - dists[v]) >> k & 1) u = parent[k][u];
        }

        if(u == v) return u;

        REPD(k, K) {
            if(parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }

        return parent[0][u];
    }

    size_type distance(const size_type u, const size_type v) const {
        return dists[u] + dists[v] - 2 * dists[find(u, v)];
    }
};


} // namespace lib

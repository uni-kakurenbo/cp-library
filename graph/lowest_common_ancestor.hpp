#pragma once

#include <vector>

#include "snippet/iterations.hpp"
#include "internal/types.hpp"

#include "graph.hpp"

namespace lib {

template<class graph = graph<>>
struct lowest_common_ancestor {
    using vertex = typename graph::vertex;
    std::vector<std::vector<vertex>> parent;
    std::vector<vertex> dists;

  private:
    void dfs(const graph &G, const vertex v, const vertex p, const vertex d) {
        parent[0][v] = p;
        dists[v] = d;
        for(const auto& e : G[v]) {
            if(e.to != p) dfs(G, e.to, v, d+1);
        }
    }

  public:
    lowest_common_ancestor(const graph &G, const vertex root = 0) { this->init(G, root); }

    void init(const graph &G, const vertex root = 0) {
        const vertex V = G.size();
        vertex K = 1; while((1 << ++K) < V);

        parent.assign(K, std::vector<vertex>(V, -1));
        dists.assign(V, -1);

        this->dfs(G, root, -1, 0);

        REP(k, K-1) REP(v, V) {
            if(parent[k][v] < 0) parent[k+1][v] = -1;
            else parent[k+1][v] = parent[k][parent[k][v]];
        }
    }

    vertex operator()(const vertex u, const vertex v) const {
        return this->find(u, v);
    }

    vertex find(vertex u, vertex v) const {
        if(dists[u] < dists[v]) std::swap(u, v);
        vertex K = parent.size();

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

    vertex edges(const vertex u, const vertex v) const {
        return dists[u] + dists[v] - 2 * dists[find(u, v)];
    }
};

} // namespace lib

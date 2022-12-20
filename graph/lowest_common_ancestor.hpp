#pragma once

#include <vector>

#include "template.hpp"
#include "internal/types.hpp"

#include "graph.hpp"

namespace Lib {

template<class Graph = Graph<>>
struct LowestCommonAncestor {
    using Vertex = typename Graph::Vertex;
    std::vector<std::vector<Vertex>> parent;
    std::vector<Vertex> dists;

  private:
    void dfs(const Graph &G, const Vertex v, const Vertex p, const Vertex d) {
        parent[0][v] = p;
        dists[v] = d;
        for(const auto& e : G[v]) {
            if(e.to != p) dfs(G, e.to, v, d+1);
        }
    }

  public:
    LowestCommonAncestor(const Graph &G, const Vertex root = 0) { this->init(G, root); }

    void init(const Graph &G, const Vertex root = 0) {
        const Vertex V = G.size();
        Vertex K = 1; while((1 << ++K) < V);

        parent.assign(K, std::vector<Vertex>(V, -1));
        dists.assign(V, -1);

        this->dfs(G, root, -1, 0);

        REP(k, K-1) REP(v, V) {
            if(parent[k][v] < 0) parent[k+1][v] = -1;
            else parent[k+1][v] = parent[k][parent[k][v]];
        }
    }

    Vertex operator()(const Vertex u, const Vertex v) const {
        return this->find(u, v);
    }

    Vertex find(Vertex u, Vertex v) const {
        if(dists[u] < dists[v]) std::swap(u, v);
        Vertex K = parent.size();

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

    Vertex edges(const Vertex u, const Vertex v) const {
        return dists[u] + dists[v] - 2 * dists[query(u, v)];
    }
};

} // namespace Lib

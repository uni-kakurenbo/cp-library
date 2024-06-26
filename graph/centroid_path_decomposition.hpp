#pragma once


#include <vector>
#include <utility>
#include <functional>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "template/debug.hpp"

namespace uni {

// Thanks to: https://kazuma8128.hatenablog.com/entry/2018/07/16/010500#fn-96e76557
class centroid_path_decomposition {
    using size_type = internal::size_t;

  private:
    std::vector<std::vector<size_type>> graph;

  public:
    std::vector<size_type> in, out, size, head, parent;

  private:
    size_type _cur = 0;

    void _erase_parent(const size_type v, const size_type p) noexcept(NO_EXCEPT) {
        this->parent[v] = p;
        ITRR(nv, graph[v]) {
            if(nv == this->graph[v].back()) break;
            if(nv == p) std::swap(nv, this->graph[v].back());
            this->_erase_parent(nv, v);
        }
        this->graph[v].pop_back();
    }

    void _race_size(const size_type v) noexcept(NO_EXCEPT) {
        ITRR(nv, this->graph[v]) {
            this->_race_size(nv);
            this->size[v] += this->size[nv];
            if(this->size[nv] > this->size[this->graph[v].front()]) std::swap(nv, this->graph[v].front());
        }
    }

    void _race_path(const size_type v) noexcept(NO_EXCEPT) {
        this->in[v] = this->_cur++;
        ITR(nv, this->graph[v]) {
            this->head[nv] = (nv == this->graph[v].front() ? this->head[v] : nv);
            this->_race_path(nv);
        }
        this->out[v] = this->_cur;
    }

  public:
    template<class Graph>
    centroid_path_decomposition(const Graph& _graph, const size_type root = 0) noexcept(NO_EXCEPT)
      : graph(_graph.size()), in(_graph.size(), -1), out(_graph.size(), -1), size(_graph.size(), 1), head(_graph.size()), parent(_graph.size(), -1)
    {
        REP(v, _graph.size()) ITR(nv, _graph[v]) { this->graph[v].push_back(nv); }
        this->build(root);
    }

    void build(const size_type root = 0) noexcept(NO_EXCEPT) {
        ITR(v, this->graph[root]) this->_erase_parent(v, root);
        this->_race_size(root);
        this->head[root] = root;
        this->_race_path(root);
    }

    size_type id(const size_type v) noexcept(NO_EXCEPT) { return this->in[v]; }

    size_type lca(size_type u, size_type v) const noexcept(NO_EXCEPT) {
        while(true) {
            if(this->in[u] > this->in[v]) std::swap(u, v);
            if(this->head[u] == this->head[v]) return u;
            v = this->parent[this->head[v]];
        }
    }

    template<class F>
    void edges_on_path(size_type u, size_type v, F&& f) noexcept(NO_EXCEPT) {
        while(true) {
            if(this->in[u] > this->in[v]) std::swap(u, v);
            if(this->head[u] != this->head[v]) {
                f(this->in[this->head[v]] - 1, this->in[v]);
                v = this->parent[this->head[v]];
            } else {
                if(u != v) f(this->in[u], this->in[v]);
                break;
            }
        }
    }

    template<class F>
    void nodes_on_path(size_type u, size_type v, F&& f) noexcept(NO_EXCEPT) {
        while (true) {
            if (this->in[u] > this->in[v]) std::swap(u, v);
            f(std::max(this->in[this->head[v]], this->in[u]), this->in[v]);
            if (this->head[u] != this->head[v])
                v = this->parent[this->head[v]];
            else {
                break;
            }
        }
    }

    template<class F>
    void subtree(const size_type v, F&& f) noexcept(NO_EXCEPT) { f(this->in[v], this->out[v]); }
};


} // namespace uni

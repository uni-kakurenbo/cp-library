#pragma once


#include <vector>
#include <utility>
#include <functional>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {

// Thanks to: https://kazuma8128.hatenablog.com/entry/2018/07/16/010500#fn-96e76557
class centroid_path_decomposition {
    using size_type = internal::size_t;

  private:
    std::vector<std::vector<size_type>> G;

  public:
    std::vector<size_type> in, out, size, head, parent;

  private:
    size_type _cur = 0;

    void _erase_parent(const size_type v, const size_type p) noexcept(NO_EXCEPT) {
        this->parent[v] = p;
        ITRR(nv, G[v]) {
            if(nv == this->G[v].back()) break;
            if(nv == p) std::swap(nv, this->G[v].back());
            this->_erase_parent(nv, v);
        }
        this->G[v].pop_back();
    }

    void _race_size(const size_type v) noexcept(NO_EXCEPT) {
        ITRR(nv, this->G[v]) {
            this->_race_size(nv);
            this->size[v] += this->size[nv];
            if(this->size[nv] > this->size[this->G[v].front()]) std::swap(nv, this->G[v].front());
        }
    }

    void _race_path(const size_type v) noexcept(NO_EXCEPT) {
        this->in[v] = this->_cur++;
        ITR(nv, this->G[v]) {
            this->head[nv] = (nv == this->G[v].front() ? this->head[v] : nv);
            this->_race_path(nv);
        }
        this->out[v] = this->_cur;
    }

  public:
    template<class graph>
    centroid_path_decomposition(const graph& G, const size_type root = 0) noexcept(NO_EXCEPT)
      : G(G.size()), in(G.size(), -1), out(G.size(), -1), size(G.size(), 1), head(G.size()), parent(G.size(), -1)
    {
        REP(v, G.size()) ITR(nv, G[v]) this->G[v].push_back(nv);
        this->build(root);
    }

    void build(const size_type root = 0) noexcept(NO_EXCEPT) {
        ITR(v, this->G[root]) this->_erase_parent(v, root);
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
    void edges_on_path(size_type u, size_type v, const F&& f) noexcept(NO_EXCEPT) {
        while(true) {
            if(this->in[u] > this->in[v]) std::swap(u, v);
            if(this->head[u] != this->head[v]) {
                f(this->in[head[v]] - 1, this->in[v]);
                v = this->parent[this->head[v]];
            } else {
                if(u != v) f(this->in[u], this->in[v]);
                break;
            }
        }
    }

    template<class F>
    void nodes_on_path(int u, int v, const F&& f) noexcept(NO_EXCEPT) {
        while (true) {
            if (this->in[u] > this->in[v]) std::swap(u, v);
            f(std::max(this->in[this->head[v]] - 1, this->in[u]), this->in[v]);
            if (this->head[u] != this->head[v])
                v = this->parent[this->head[v]];
            else {
                break;
            }
        }
    }

    template<class F>
    void subtree(const size_type v, const F&& f) noexcept(NO_EXCEPT) { f(this->in[v], this->out[v]); }
};


} // namespace lib

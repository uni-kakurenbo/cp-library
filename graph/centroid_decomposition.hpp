#pragma once

#include <cassert>
#include <vector>
#include <utility>

#include "internal/dev_env.hpp"
#include "structure/graph.hpp"

namespace uni {


// Thanks to: https://qiita.com/drken/items/4b4c3f1824339b090202
template<class Graph = graph<>>
struct centroid_decomposition {
    using size_type = internal::size_t;

    std::vector<size_type> centroids;

  private:
    const Graph& graph;
    std::vector<size_type> _size, _parent;
    std::vector<bool> _used;

  public:
    centroid_decomposition(const Graph& _graph) noexcept(NO_EXCEPT)
      : graph(_graph),
        _size(graph.vertices()), _parent(graph.vertices()), _used(graph.vertices())
    {}

    inline const auto& sizes() const noexcept(NO_EXCEPT) { return this->_size; }
    inline const auto& parents() const noexcept(NO_EXCEPT) { return this->_parent; }
    inline const auto& used() const noexcept(NO_EXCEPT) { return this->_used; }

    inline size_type size(const size_type v) const noexcept(NO_EXCEPT) {
        assert(0 <= v && v < this->graph.vertices());
        return this->_size[v];
    }
    inline size_type parent(const size_type v) const noexcept(NO_EXCEPT) {
        assert(0 <= v && v < this->graph.vertices());
        return this->_parent[v];
    }
    inline bool used(const size_type v) const noexcept(NO_EXCEPT) {
        assert(0 <= v && v < this->graph.vertices());
        return this->_used[v];
    }

    const std::vector<size_type>& find(const size_type v, const size_type sz, const size_type p = -1) noexcept(NO_EXCEPT) {
        assert(not this->_used[v]);

        this->_size[v] = 1, this->_parent[v] = p;
        bool found = true;
        ITR(e, this->graph[v]) {
            if(e.to == p) continue;
            if(this->_used[e.to]) continue;

            this->find(e.to, sz, v);
            if(this->_size[e.to] > sz / 2) found = false;
            this->_size[v] += this->_size[e.to];
        }
        if(sz - this->_size[v] > sz / 2) found = false;
        if(found) this->centroids.push_back(v);

        return this->centroids;
    }

    auto decompose(const size_type root, const size_type sz) noexcept(NO_EXCEPT) {
    assert(not this->_used[root]);

        std::vector<std::pair<size_type,size_type>> subtrees;

        this->centroids.clear();
        this->find(root, sz);

        const size_type centroid = this->centroids[0];
        this->_used[centroid] = true;

        ITR(e, this->graph[centroid]) {
            if(this->_used[e.to]) continue;
            if(e.to == this->_parent[centroid]) {
                subtrees.emplace_back(e.to, sz - this->_size[centroid]);
            }
            else {
                subtrees.emplace_back(e.to, this->_size[e.to]);
            }
        }

        return std::make_pair(centroid, subtrees);
    }

    auto decompose(const size_type root = 0) noexcept(NO_EXCEPT) {
        return this->decompose(root, this->graph.vertices());
    }
};


} // namespace uni

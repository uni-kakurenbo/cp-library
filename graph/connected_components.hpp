#pragma once

#include <vector>
#include <queue>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "structure/graph.hpp"
#include "data_structure/disjoint_set.hpp"

template<class Graph>
uni::disjoint_set uni::internal::graph_impl::mixin<Graph>::components() const noexcept(NO_EXCEPT) {
    uni::disjoint_set disjoint_set(this->vertices());
    ITR(edges, *this) ITR(_id, u, v, _w, _idx, edges) {
        disjoint_set.merge(u, v);
    }
    return disjoint_set;
}

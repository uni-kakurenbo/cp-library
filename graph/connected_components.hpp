#pragma once

#include <vector>
#include <queue>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "structure/graph.hpp"
#include "data_structure/disjoint_set_union.hpp"

template<class Graph>
uni::dsu uni::internal::graph_impl::mixin<Graph>::components() const noexcept(NO_EXCEPT) {
    uni::dsu dsu(this->vertices());
    ITR(edges, *this) ITR(_id, u, v, _w, _idx, edges) {
        dsu.merge(u, v);
    }
    return dsu;
}

#pragma once

#include <vector>
#include <queue>

#include "snippet/iterations.hpp"
// #include "snippet/internal/types.hpp"

#include "graph.hpp"
#include "data_structure/disjoint_set_union.hpp"

template<class edge_cost>
typename lib::graph<edge_cost>::vertex lib::graph<edge_cost>::count_components() const {
    lib::dsu dsu(this->vertexes());
    ITR(edges, *this) ITR(u, v, _w, edges) {
        dsu.merge(u, v);
    }
    return dsu.group_count();
}

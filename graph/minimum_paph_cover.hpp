#pragma once

#include <memory>

#include "snippet/iterations.hpp"
#include "internal/dev_env.hpp"

#include "graph.hpp"
#include "graph/maximum_bipartite_matching.hpp"

template<class edge_cost>
typename lib::graph<edge_cost>::size_type lib::graph<edge_cost>::minimum_paph_cover_size_as_dag() const noexcept(DEV_ENV) {
    lib::maximum_bipartite_matching bm(this->size());

    REP(i, this->size()) ITR(j, (*this)[i]) {
        bm.add(i, j.to);
    }

    return this->size() - bm.solve();
}

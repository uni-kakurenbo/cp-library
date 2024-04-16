#pragma once


#include <memory>

#include "snippet/iterations.hpp"
#include "internal/dev_env.hpp"

#include "structure/graph.hpp"
#include "graph/maximum_bipartite_matching.hpp"


template<class Graph>
typename uni::internal::graph_impl::mixin<Graph>::size_type uni::internal::graph_impl::mixin<Graph>::minimum_paph_cover_size_as_dag() const noexcept(NO_EXCEPT) {
    uni::maximum_bipartite_matching bm(this->size());

    REP(i, this->size()) ITR(j, (*this)[i]) {
        bm.add(i, j.to);
    }

    return this->size() - bm.max_matched();
}

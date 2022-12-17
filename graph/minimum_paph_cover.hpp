#pragma once

#include <memory>

#include "template.hpp"

#include "graph.hpp"
#include "graph/maximum_bipartite_matching.hpp"

template<class E>
typename Lib::Graph<E>::Vertex Lib::Graph<E>::minimum_paph_cover_size_as_dag() const {
    Lib::MaximumBipartiteMatching bm(this->size());

    REP(i, this->size()) ITR(j, (*this)[i]) {
        bm.add(i, j.to);
    }

    return this->size() - bm.solve();
}

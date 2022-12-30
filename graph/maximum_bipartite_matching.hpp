#pragma once

#include <atcoder/maxflow>

#include "snippet/iterations.hpp"
#include "internal/types.hpp"

namespace lib {

struct maximum_bipartite_matching {
    using vertex = internal::size_t;

  protected:
    using MF = atcoder::mf_graph<vertex>;

    vertex _n;
    MF mf;

  public:
    maximum_bipartite_matching(vertex n = 0) : _n(n), mf(2*n+2) {
        REP(i, n) {
            this->mf.add_edge(2*n, i, 1);
            this->mf.add_edge(n+i, 2*n+1, 1);
        }
    }

    void add(vertex i, vertex j) {
        this->mf.add_edge(i, this->_n+j, 1);
    }

    vertex solve() {
        return this->mf.flow(2*this->_n, 2*this->_n+1);
    }
};

} // namespace lib

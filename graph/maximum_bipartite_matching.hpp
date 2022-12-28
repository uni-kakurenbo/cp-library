#pragma once

#include <atcoder/maxflow>

#include "snippet/iterations.hpp"
#include "internal/types.hpp"

namespace Lib {

struct MaximumBipartiteMatching {
    using Vertex = Internal::Size;

  protected:
    using MF = atcoder::mf_graph<Vertex>;

    Vertex _n;
    MF mf;

  public:
    MaximumBipartiteMatching(Vertex n = 0) : _n(n), mf(2*n+2) {
        REP(i, n) {
            this->mf.add_edge(2*n, i, 1);
            this->mf.add_edge(n+i, 2*n+1, 1);
        }
    }

    void add(Vertex i, Vertex j) {
        this->mf.add_edge(i, this->_n+j, 1);
    }

    Vertex solve() {
        return this->mf.flow(2*this->_n, 2*this->_n+1);
    }
};

} // namespace Lib

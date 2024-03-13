#pragma once


#include <atcoder/maxflow>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adapter/vector.hpp"


namespace lib {

struct maximum_bipartite_matching {
    // using size_type = internal::size_t;
    using size_type = int;

  protected:
    using MF = atcoder::mf_graph<size_type>;

    size_type _m, _n, _s, _edges = 0;
    MF _mf;

  public:
    explicit maximum_bipartite_matching(const size_type n) noexcept(NO_EXCEPT) : maximum_bipartite_matching(n, n) {}
    maximum_bipartite_matching(const size_type m, const size_type n) noexcept(NO_EXCEPT)
      : _m(m), _n(n), _s(m + n), _mf(m + n + 2)
    {
        REP(i, m) {
            this->_mf.add_edge(this->_s, i, 1);
        }
        REP(i, n) {
            this->_mf.add_edge(m + i, this->_s + 1, 1);
        }
    }

    inline auto& add(const size_type i, const size_type j) noexcept(NO_EXCEPT) {
        assert(0 <= i && i < this->_m);
        assert(0 <= j && j < this->_n);
        this->_mf.add_edge(i, this->_m + j, 1);
        ++this->_edges;
        return *this;
    }

    inline size_type max_matched() noexcept(NO_EXCEPT) {
        return this->_mf.flow(this->_s, this->_s + 1);
    }

    inline auto get_matching() noexcept(NO_EXCEPT) {
        this->max_matched();

        vector<spair<size_type>> res;

        REP(i, this->_edges) {
            const auto edge = this->_mf.get_edge(this->_s + i);
            if(edge.flow == 0) continue;
            res.emplace_back(edge.from, edge.to - this->_m);
        }

        return res;
    }
};

} // namespace lib

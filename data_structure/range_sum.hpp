#pragma once

#include <iterator>
#include <initializer_list>

#include <atcoder/fenwicktree>

#include "template.hpp"
#include "internal/develop/dev_assert.hpp"

template<class T> struct RangeSum : atcoder::fenwick_tree<T> {
  protected:
    int _n;

  public:
    RangeSum(const int n = 0) : atcoder::fenwick_tree<T>(n+1) { this->_n = n; }
    RangeSum(std::initializer_list<T> init_list) : RangeSum(ALL(init_list)) {}

    template<class I>
    RangeSum(const I first, const I last) : RangeSum(std::distance(first, last)) {
        for(auto itr=first; itr!=last; ++itr) this->set(itr-first, *itr);
    }

    inline int size() const {
        return this->_n;
    }
    inline T get(const int p) {
        dev_assert(0 <= p and p < this->size());
        return this->sum(p, p+1);
    }

    inline void set(const int p, const T x) {
        dev_assert(0 <= p and p < this->size());
        this->add(p, x - this->get(p));
    }

    inline std::vector<T> _debug() {
        std::vector<T> res(this->size());
        REP(i, this->size()) res[i] = this->get(i);
        return res;
    }
};

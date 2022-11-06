#pragma once

#include <memory>
#include <numeric>
#include <algorithm>

#include "data_structure/range_sum.hpp"
#include "iterable/adjacent_difference.hpp"

namespace Lib {

template<class T> struct RangeAddition {
  protected:
    RangeSum<T> difference;
    using Size = int;

  private:
    Size _n;


  public:
    RangeAddition(const Size n = 0) : difference(n) { this->_n = n; }
    RangeAddition(std::initializer_list<T> init_list) : RangeAddition(ALL(init_list)) {}

    template<class I> RangeAddition(const I first, const I last) : RangeAddition(std::distance(first, last)) {
        AdjacentDifference<T> diff(first, last, false);
        REP(i, diff.size()) this->difference.set(i, diff[i]);
    }

    inline Size size() const { return this->_n; }

    inline void add(const Size l, const Size r, const T v) {
        dev_assert(0 <= l and l <= r and r <= this->size());
        this->difference.add(l, v);
        this->difference.add(r, -v);
    }
    inline T get(const Size p) {
        dev_assert(0 <= p and p < this->size());
        return this->difference.sum(0, p+1);
    }

    inline std::vector<T> _debug() {
        std::vector<T> res(this->size());
        REP(i, this->size()) res[i] = this->get(i);
        return res;
    }
};

} // namespace Lib

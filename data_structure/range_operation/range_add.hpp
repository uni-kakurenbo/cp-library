#pragma once

#include "data_structure/range_operation/range_sum.hpp"

namespace Lib {


// Implemented by Fenwick Tree
template<class T> struct RangeAdd : RangeSum<T> {
    using Size = typename RangeSum<T>::Size;
    using RangeSum<T>::RangeSum;

    RangeAdd(const Size n = 0) : RangeSum<T>(n) {}
    RangeAdd(std::initializer_list<T> init_list) : RangeAdd(ALL(init_list)) {}

    template<class I>
    RangeAdd(const I first, const I last) : RangeAdd(std::distance(first, last)) {
        AdjacentDifference<T> diff(first, last, false);
        REP(i, diff.size()) this->set(i, diff[i]);
    }

    inline void add(const Size first, const Size last, const T& v) {
        dev_assert(0 <= first and first <= last and last <= this->size());
        this->add(first, v), this->add(last, -v);
    }
    inline void add(const Size p, const T& v) { this->add(p, p+1, v); }
    inline void add(const T& v) { this->add(0, this->size(), v); }

    inline T get(const Size p) const {
        dev_assert(0 <= p and p < this->size());
        return this->prod(p+1);
    }
    inline T operator[](const Size pos) const { return this->get(pos); }
};


} // namespace Lib

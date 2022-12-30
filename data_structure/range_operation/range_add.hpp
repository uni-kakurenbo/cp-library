#pragma once

#include "data_structure/range_operation/range_sum.hpp"

namespace lib {


// Implemented by Fenwick Tree
template<class T> struct range_add : range_sum<T> {
    using size_t = typename range_sum<T>::size_t;
    using range_sum<T>::range_sum;

    range_add(const size_t n = 0) : range_sum<T>(n) {}
    range_add(std::initializer_list<T> init_list) : range_add(ALL(init_list)) {}

    template<class I>
    range_add(const I first, const I last) : range_add(std::distance(first, last)) {
        adjacent_difference<T> diff(first, last, false);
        REP(i, diff.size()) this->set(i, diff[i]);
    }

    inline void add(const size_t first, const size_t last, const T& v) {
        dev_assert(0 <= first and first <= last and last <= this->size());
        this->add(first, v), this->add(last, -v);
    }
    inline void add(const size_t p, const T& v) { this->add(p, p+1, v); }
    inline void add(const T& v) { this->add(0, this->size(), v); }

    inline T get(const size_t p) const {
        dev_assert(0 <= p and p < this->size());
        return this->prod(p+1);
    }
    inline T operator[](const size_t pos) const { return this->get(pos); }
};


} // namespace lib

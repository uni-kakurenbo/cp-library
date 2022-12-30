#pragma once

#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

#include "internal/types.hpp"
#include "internal/dev_assert.hpp"

namespace lib {

template<class T, class container = std::vector<T>>
struct accumulation : container {
  protected:
    inline internal::size_t _positivize_index(const internal::size_t x) const {
        return x < 0 ? this->size() + x : x;
    }

  public:
    accumulation() {}

    template<class I, class Operator = std::plus<T>>
    accumulation(const I first, const I last, const T head = T{}, const Operator op = std::plus<T>{}) {
        this->resize(distance(first, last));
        std::exclusive_scan(first, last, begin(*this), head, op);
        this->push_back(op(*rbegin(*this), *prev(last)));
    }

    template<class Operaotr = std::minus<T>>
    inline T operator()(const internal::size_t _left, const internal::size_t _right, Operaotr op = std::minus<T>{}) const {
        const internal::size_t left = _positivize_index(_left);
        const internal::size_t right = _positivize_index(_right);
        dev_assert(0 <= left and left < (internal::size_t)this->size()), dev_assert(0 <= right and right < (internal::size_t)this->size());
        return op((*this)[right], (*this)[left]);
    }
};

} // namespace lib

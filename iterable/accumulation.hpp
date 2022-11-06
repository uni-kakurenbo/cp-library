#pragma once

#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

#include "internal/types.hpp"
#include "internal/develop/dev_assert.hpp"

namespace Lib {

template<class T, class Container = std::vector<T>>
struct Accumulation : Container {
  protected:
    inline Internal::Size _positivize_index(const Internal::Size x) const {
        return x < 0 ? this->size() + x : x;
    }

  public:
    Accumulation() {}

    template<class I, class Operator = std::plus<T>>
    Accumulation(const I first, const I last, const T head = T{}, const Operator op = std::plus<T>{}) {
        this->resize(distance(first, last));
        std::exclusive_scan(first, last, begin(*this), head, op);
        this->push_back(op(*rbegin(*this), *prev(last)));
    }

    template<class Operaotr = std::minus<T>>
    inline T operator()(const Internal::Size _left, const Internal::Size _right, Operaotr op = std::minus<T>{}) const {
        const Internal::Size left = _positivize_index(_left);
        const Internal::Size right = _positivize_index(_right);
        dev_assert(0 <= left and left < (Internal::Size)this->size()), dev_assert(0 <= right and right < (Internal::Size)this->size());
        return op((*this)[right], (*this)[left]);
    }
};

} // namespace Lib

#pragma once

#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

#include "internal/types.hpp"

template<class I, class T = typename std::iterator_traits<I>::value_type, class Container = std::vector<T>>
struct AdjacentDifference : Container {
  public:
    AdjacentDifference() {}

    template<class Operator = std::minus<T>>
    AdjacentDifference(const I first, const I last, const Operator op = std::minus<T>{}) {
        this->resize(distance(first, last));
        std::adjacent_difference(first, last, begin(*this), op);
        this->erase(begin(*this));
    }
};

#pragma once

#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

#include "internal/types.hpp"

namespace Lib {

template<class T, class Container = std::vector<T>>
struct AdjacentDifference : Container {
  public:
    AdjacentDifference() {}

    template<class I, class Operator = std::minus<T>>
    AdjacentDifference(const I first, const I last, const bool remove_first = true, const Operator op = std::minus<T>{}) {
        this->resize(distance(first, last));
        std::adjacent_difference(first, last, begin(*this), op);
        if(remove_first) this->erase(begin(*this));
    }
};

} // namespace Lib

#pragma once

#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

namespace lib {

template<class T = i64, class container = std::vector<T>>
struct adjacent_difference : container {
  public:
    adjacent_difference() noexcept(DEV_ENV) {}

    template<class I, class Operator = std::minus<T>>
    adjacent_difference(const I first, const I last, const bool remove_first = true, const Operator op = std::minus<T>{}) noexcept(DEV_ENV) {
        this->resize(std::distance(first, last));
        std::adjacent_difference(first, last, begin(*this), op);
        if(remove_first) this->erase(begin(*this));
    }
};

} // namespace lib

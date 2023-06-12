#pragma once


#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {


template<class T, class container = std::vector<T>>
struct adjacent_difference : container {
  public:
    explicit adjacent_difference() noexcept(NO_EXCEPT) {}

    template<class I, class Operator = std::minus<T>>
    explicit adjacent_difference(const I first, const I last, const bool remove_first = true, const Operator op = std::minus<T>{}) noexcept(NO_EXCEPT) {
        this->resize(std::distance(first, last));
        std::adjacent_difference(first, last, begin(*this), op);
        if(remove_first) this->erase(begin(*this));
    }
};

template<class I>
explicit adjacent_difference(const I, const I) -> adjacent_difference<typename std::iterator_traits<I>::value_type>;


} // namespace lib

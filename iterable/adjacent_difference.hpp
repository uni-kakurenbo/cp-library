#pragma once


#include <iterator>
#include <functional>
#include <numeric>
#include <vector>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adapter/valarray.hpp"


namespace lib {


template<class T, class container = valarray<T>>
struct adjacent_difference : container {
  public:
    explicit adjacent_difference() noexcept(NO_EXCEPT) {}

    template<class I, class Operator = std::minus<T>>
    explicit adjacent_difference(const I first, const I last, const bool remove_first = true, const Operator op = std::minus<T>{}) noexcept(NO_EXCEPT) {
        this->resize(std::distance(first, last));
        std::vector<T> diff(this->size());
        std::adjacent_difference(first, last, begin(diff), op);
        if(remove_first) diff.erase(begin(diff));
        this->assign(std::begin(diff), std::end(diff));
    }
};

template<class I>
explicit adjacent_difference(const I, const I) -> adjacent_difference<typename std::iterator_traits<I>::value_type>;


} // namespace lib

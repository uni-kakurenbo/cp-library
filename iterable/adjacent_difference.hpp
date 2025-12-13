#pragma once


#include <iterator>
#include <functional>
#include <numeric>
#include <vector>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adaptor/valarray.hpp"


namespace uni {


template<class T, class container = valarray<T>>
struct adjacent_difference : container {
  public:
    explicit adjacent_difference() noexcept(NO_EXCEPT) {}

    template<std::ranges::input_range R>
    explicit adjacent_difference(R&& r) noexcept(NO_EXCEPT) : adjacent_difference(ALL(r)) {}

    template<
        std::input_iterator I, std::sentinel_for<I> S,
        class Operator = std::minus<T>
    >
    explicit adjacent_difference(I first, S last, const bool remove_first = true, const Operator op = std::minus<T>{}) noexcept(NO_EXCEPT) {
        this->resize(std::ranges::distance(first, last));
        std::vector<T> diff(this->size());
        std::adjacent_difference(first, last, std::ranges::begin(diff), op);
        if(remove_first) diff.erase(std::ranges::begin(diff));
        this->assign(std::ranges::begin(diff), std::ranges::end(diff));
    }
};


template<std::input_iterator I, std::sentinel_for<I> S>
explicit adjacent_difference(I, S) -> adjacent_difference<std::iter_value_t<I>>;


template<std::ranges::input_range R>
explicit adjacent_difference(R&&) -> adjacent_difference<std::ranges::range_value_t<R>>;


} // namespace uni

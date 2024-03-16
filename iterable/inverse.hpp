#pragma once

#include <iterator>
#include <vector>

#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adaptor/vector.hpp"

#include "internal/auto_holder.hpp"


namespace lib {


template<class T, class V = vector<internal::size_t>, class container = dynamic_auto_holder<T, V>>
struct inverse : container {
    explicit inverse() noexcept(NO_EXCEPT) {}
    template<class R> inverse(R&& range) noexcept(NO_EXCEPT) : inverse(std::ranges::begin(range), std::ranges::end(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    inverse(I first, S last) noexcept(NO_EXCEPT) {
        typename V::value_type index = 0;
        for(auto itr = first; itr != last; ++itr, ++index) (*this)[*itr].emplace_back(index);
    }
};

template<std::input_iterator I, std::sentinel_for<I> S>
explicit inverse(I, S) -> inverse<typename std::iterator_traits<I>::value_type>;

template<class R>
explicit inverse(R&&) -> inverse<typename R::value_type>;


} // namespace lib

#pragma once

#include <iterator>

#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "internal/auto_holder.hpp"


namespace lib {


template<class T, class container = dynamic_auto_holder<T,internal::size_t>>
struct counter : container {
    counter() noexcept(NO_EXCEPT) = default;

    template<std::input_iterator I, std::sentinel_for<I> S>
    counter(I first, S last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }

    template<std::ranges::input_range R>
    explicit counter(R&& range) noexcept(NO_EXCEPT) : counter(ALL(range)) {}
};


template<std::input_iterator I, std::sentinel_for<I> S>
explicit counter(I, S) -> counter<std::iter_value_t<I>>;

template<std::ranges::input_range R>
explicit counter(R) -> counter<std::ranges::range_value_t<R>>;


} // namespace lib

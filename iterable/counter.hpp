#pragma once

#include <iterator>

#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "internal/auto_holder.hpp"


namespace lib {


template<class T, class container = dynamic_auto_holder<T,internal::size_t>>
struct counter : container {
    explicit counter() noexcept(NO_EXCEPT) {}
    template<std::input_iterator I, std::sentinel_for<I> S>
    counter(I first, S last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }
};

template<std::input_iterator I, std::sentinel_for<I> S>
explicit counter(I, S) -> counter<typename std::iterator_traits<I>::value_type>;


} // namespace lib

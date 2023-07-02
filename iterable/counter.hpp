#pragma once

#include <iterator>
#include <map>

#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {


template<class T, class container = std::map<T,internal::size_t>>
struct counter : container {
    explicit counter() noexcept(NO_EXCEPT) {}
    template<class I> counter(const I first, const I last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }
};

template<class I>
explicit counter(const I, const I) -> counter<typename std::iterator_traits<I>::value_type>;


} // namespace lib

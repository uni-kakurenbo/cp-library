#pragma once

#include <iterator>
#include <unordered_map>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

namespace lib {

template<class T = i64, class container = std::unordered_map<T,internal::size_t>>
struct counter : container {
    counter() noexcept(NO_EXCEPT) {}
    template<class I> counter(const I first, const I last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }
};

} // namespace lib

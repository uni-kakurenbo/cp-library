#pragma once

#include <iterator>
#include <unordered_map>

#include "internal/types.hpp"

namespace lib {

template<class T = i64, class container = std::unordered_map<T,internal::size_t>>
struct counter : container {
    counter() {}
    template<class I> counter(const I first, const I last) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }
};

} // namespace lib

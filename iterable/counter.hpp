#pragma once

#include <iterator>
#include <unordered_map>

#include "internal/types.hpp"

namespace Lib {

template<class T, class Container = std::unordered_map<T,Internal::Size>>
struct Counter : Container {
    Counter() {}
    template<class I> Counter(const I first, const I last) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }
};

} // namespace Lib

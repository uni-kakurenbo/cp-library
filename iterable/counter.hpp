#pragma once

#include <iterator>
#include <unordered_map>

#include "internal/types.hpp"

template<class I, class T = typename std::iterator_traits<I>::value_type, class Container = std::unordered_map<T,Internal::Size>>
struct Counter : Container {
    Counter() {}
    Counter(const I first, const I last) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }
};

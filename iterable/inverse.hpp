#pragma once

#include <iterator>
#include <vector>
#include <unordered_map>

#include "internal/types.hpp"

namespace Lib {

template<class I, class T = typename std::iterator_traits<I>::value_type, class V = std::vector<Internal::Size>, class Container = std::unordered_map<T,V>>
struct Inverse : Container {
    Inverse() {}
    Inverse(const I first, const I last) {
        for(auto itr=first; itr!=last; ++itr) (*this)[*itr].push_back(std::distance(first,itr));
    }
};

} // namespace Lib

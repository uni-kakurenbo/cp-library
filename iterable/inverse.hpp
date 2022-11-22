#pragma once

#include <iterator>
#include <vector>
#include <unordered_map>

#include "internal/types.hpp"

namespace Lib {

template<class T, class V = std::vector<Internal::Size>, class Container = std::unordered_map<T,V>>
struct Inverse : Container {
    Inverse() {}
    template<class I> Inverse(const I first, const I last) {
        for(auto itr=first; itr!=last; ++itr) (*this)[*itr].push_back(std::distance(first,itr));
    }
};

} // namespace Lib

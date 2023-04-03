#pragma once

#include <iterator>
#include <vector>
#include <unordered_map>

#include "internal/types.hpp"

namespace lib {

template<class T = i64, class V = std::vector<internal::size_t>, class container = std::unordered_map<T,V>>
struct inverse : container {
    inverse() {}
    template<class I> inverse(const I first, const I last) {
        for(auto itr=first; itr!=last; ++itr) (*this)[*itr].push_back(std::distance(first,itr));
    }
};

} // namespace lib

#pragma once

#include <iterator>
#include <vector>
#include <unordered_map>

#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"

namespace lib {

template<class T = i64, class V = std::vector<internal::size_t>, class container = std::unordered_map<T,V>>
struct inverse : container {
    inverse() noexcept(NO_EXCEPT) {}
    template<class I> inverse(const I first, const I last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr!=last; ++itr) (*this)[*itr].push_back(std::distance(first,itr));
    }
};

} // namespace lib

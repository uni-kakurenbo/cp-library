#pragma once

#include <iterator>
#include <vector>
#include <unordered_map>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

namespace lib {

template<class T = i64, class V = std::vector<internal::size_t>, class container = std::unordered_map<T,V>>
struct inverse : container {
    inverse() noexcept(DEV_ENV) {}
    template<class I> inverse(const I first, const I last) noexcept(DEV_ENV) {
        for(auto itr=first; itr!=last; ++itr) (*this)[*itr].push_back(std::distance(first,itr));
    }
};

} // namespace lib

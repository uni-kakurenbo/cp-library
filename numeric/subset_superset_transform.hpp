#pragma once


#include <bit>
#include <ranges>

#include "snippet/aliases.hpp"


namespace lib {


namespace superset_transform {


template<std::ranges::random_access_range R, std::unsigned_integral Bit = std::uint32_t>
void zeta(R& range) {
    const auto n = std::ranges::size(range);
    assert(std::has_single_bit(n));

    for(Bit i = 1; i < n; i <<= 1) {
        for(Bit j = 0; j < n; ++j) {
            if((j & i) == 0) {
                range[j] += range[j | i];
            }
        }
    }
}



template<std::ranges::random_access_range R, std::unsigned_integral Bit = std::uint32_t>
void mobius(R& range) {
    const auto n = std::ranges::size(range);
    assert(std::has_single_bit(n));

    for(Bit i = 1; i < n; i <<= 1) {
        for(Bit j = 0; j < n; ++j) {
            if((j & i) == 0) {
                range[j] -= range[j | i];
            }
        }
    }
}


} // namespace superset_transform


namespace subset_transform {


template<std::ranges::random_access_range R, std::unsigned_integral Bit = std::uint32_t>
void zeta(R& range) {
    const auto n = std::ranges::size(range);
    assert(std::has_single_bit(n));

    for(Bit i = 1; i < n; i <<= 1) {
        for(Bit j = 0; j < n; ++j) {
            if((j & i) == 0) {
                range[j | i] += range[j];
            }
        }
    }
}



template<std::ranges::random_access_range R, std::unsigned_integral Bit = std::uint32_t>
void mobius(R& range) {
    const auto n = std::ranges::size(range);
    assert(std::has_single_bit(n));

    for(Bit i = 1; i < n; i <<= 1) {
        for(Bit j = 0; j < n; ++j) {
            if((j & i) == 0) {
                range[j | i] -= range[j];
            }
        }
    }
}


} // namespace subset_transform


} // namespace lib

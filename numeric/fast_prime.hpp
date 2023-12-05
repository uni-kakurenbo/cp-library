#pragma once


#include "numeric/internal/primality_test.hpp"
#include "numeric/internal/factorize.hpp"
#include "numeric/internal/divisors.hpp"


namespace lib {


constexpr i64 INTERNAL_MODINT_ID = -(1UL << 62);


inline constexpr bool is_prime(const u64 n) noexcept(NO_EXCEPT) {
    return is_prime<dynamic_modint_32bit<INTERNAL_MODINT_ID>, dynamic_modint_64bit<INTERNAL_MODINT_ID>>(n);
}


template<u64 N>
inline constexpr bool is_prime() noexcept(NO_EXCEPT) {
    return is_prime<static_modint_32bit<N>, static_modint_64bit<N>>(N);
}


} // namespace lib

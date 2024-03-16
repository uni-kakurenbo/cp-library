#pragma once


#include <cassert>
#include <ranges>


#include "adaptor/vector.hpp"
#include "adaptor/set.hpp"
#include "adaptor/map.hpp"

#include "numeric/internal/primality_test.hpp"
#include "numeric/internal/factorize.hpp"
#include "numeric/internal/divisors.hpp"
#include "numeric/internal/primitive_root.hpp"
#include "numeric/modular/modint.hpp"


namespace lib {

namespace internal {


constexpr i64 INTERNAL_MODINT_ID = -(1L << 62);


inline constexpr bool is_prime(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    return is_prime<lib::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, lib::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>>(n);
}

inline auto factorize(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    vector<i64> res;
    factorize<lib::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, lib::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>>(n, &res);
    return res;
}

inline auto divisors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    vector<i64> res;
    divisors<lib::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, lib::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>>(n, &res);
    std::ranges::sort(res);
    return res;
}

inline auto primitive_root(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    return primitive_root<lib::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, lib::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>, i64>(n);
}


} // namespace internal


using internal::is_prime;
using internal::divisors;
using internal::primitive_root;


inline vector<i64> factorize(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    auto res = internal::factorize(n);
    std::ranges::sort(res);
    return res;
}

inline set<i64> prime_factors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    const auto factors = factorize(n);
    set<i64> res(ALL(factors));
    return res;
}

inline map<i64,i64> count_factors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    map<i64,i64> mp;
    for(auto &x : internal::factorize(n)) mp[x]++;
    return mp;
}


} // namespace lib

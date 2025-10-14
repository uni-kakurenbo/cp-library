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


namespace uni {

namespace internal {


constexpr i64 INTERNAL_MODINT_ID = -(1L << 62);


inline constexpr bool is_prime(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    return is_prime<uni::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, uni::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>>(n);
}

inline auto factorize(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    vector<i64> res;
    factorize<uni::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, uni::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>>(n, &res);
    return res;
}

inline auto divisors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    vector<i64> res;
    divisors<uni::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, uni::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>>(n, &res);
    std::ranges::sort(res);
    return res;
}

template<bool FORCE_RANDOM = false>
inline auto primitive_root(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    return primitive_root<uni::dynamic_montgomery_modint_32bit<INTERNAL_MODINT_ID>, uni::dynamic_montgomery_modint_64bit<INTERNAL_MODINT_ID>, i64, FORCE_RANDOM>(n);
}


} // namespace internal


using internal::is_prime;
using internal::divisors;
using internal::primitive_root;

inline auto signed_divisors(const i64 n) noexcept(NO_EXCEPT) {
    auto divs = internal::divisors(std::abs(n));
    auto res = divs; res.reverse();
    for(const auto& div: divs) res.emplace_back(-div);
    return res.reverse();
}

inline auto factorize(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    auto res = internal::factorize(n);
    std::ranges::sort(res);
    return res;
}

inline auto prime_factors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    const auto factors = factorize(n);
    set<i64> res(ALL(factors));
    return res;
}

inline auto count_factors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    map<i64, i64> mp;
    for(auto &x : internal::factorize(n)) mp[x]++;
    return mp;
}


} // namespace uni

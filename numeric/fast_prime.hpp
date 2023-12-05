#pragma once

// #include "../inner/inner_math.hpp"
// #include "../misc/rng.hpp"

#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <random>
#include <ranges>


#include "snippet/internal/types.hpp"

#include "internal/dev_env.hpp"

#include "numeric/internal/primality_test.hpp"
#include "numeric/modint.hpp"

#include "random/xorshift.hpp"

#include "adapter/set.hpp"
#include "adapter/map.hpp"
#include "adapter/vector.hpp"


#include <atcoder/modint>

namespace lib {

namespace internal {


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {

using internal::is_prime;

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


} // namespace fast_factorize_impl

} // namespace internal


using internal::fast_factorize_impl::divisors;
using internal::fast_factorize_impl::factorize;
using internal::fast_factorize_impl::count_factors;
using internal::fast_factorize_impl::prime_factors;
using internal::fast_factorize_impl::is_prime;


} // namespace lib

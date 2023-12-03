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

namespace internal {


constexpr i64 INTERNAL_MODINT_ID = -(1UL << 62);


inline constexpr bool is_prime(const u64 n) noexcept(NO_EXCEPT) {
    return is_prime<dynamic_modint_32bit<INTERNAL_MODINT_ID>, dynamic_modint_64bit<INTERNAL_MODINT_ID>>(n);
}


template<u64 N>
inline constexpr bool is_prime() noexcept(NO_EXCEPT) {
    return is_prime<static_modint_32bit<N>, static_modint_64bit<N>>(N);
}


// Pollard's rho algorithm
template<dynamic_modint_family mint, class T>
T find_factor(const T n) noexcept(NO_EXCEPT) {
    if(~n & 1) return 2;
    if(is_prime<mint>(n)) return n;

    assert(static_cast<u64>(mint::mod()) == n);
    mint rr, one = 1;

    auto f = [&](mint x) noexcept(NO_EXCEPT) { return x * x + rr; };

    static xorshift64 rand(std::random_device{}());
    auto rand_ = [&]() noexcept(NO_EXCEPT) { return rand() % (n - 2) + 2; };

    while(true) {
        mint x, y, ys, q = one;
        rr = rand_(), y = rand_();
        T g = 1;
        constexpr int m = 128;

        for(int r = 1; g == 1; r <<= 1) {
            x = y;
            for(int i = 0; i < r; ++i) y = f(y);
            for(int k = 0; g == 1 && k < r; k += m) {
                ys = y;
                for(int i = 0; i < m && i < r - k; ++i) q *= x - (y = f(y));
                g = binary_gcd(q.val(), n);
            }
        }

        if(g == n) {
            do {
                g = binary_gcd((x - (ys = f(ys))).val(), n);
            } while(g == 1);
        }
        if(g != n) return g;
    }

    assert(false);
}


vector<i64> factorize(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    if(n <= 1) return {};

    u64 p;
    if(n <= dynamic_modint_32bit<>::max()) p = find_factor<dynamic_modint_32bit<INTERNAL_MODINT_ID>>(static_cast<u32>(n));
    else p = find_factor<dynamic_modint_64bit<INTERNAL_MODINT_ID>, u64>(n);

    if(p == static_cast<u64>(n)) return { static_cast<i64>(p) };

    auto l = internal::factorize(p);
    auto r = internal::factorize(n / p);

    std::ranges::copy(r, std::back_inserter(l));

    return l;
}


} // namespace internal


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

inline vector<i64> divisors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    if(n == 0) return {};

    std::vector<std::pair<i64, i64>> v;
    for(auto &p : factorize(n)) {
        if(v.empty() || v.back().first != p) {
            v.emplace_back(p, 1);
        } else {
            v.back().second++;
        }
    }

    vector<i64> res;
    auto f = [&](auto rc, int i, i64 x) noexcept(NO_EXCEPT) -> void {
        if(i == static_cast<int>(v.size())) {
            res.push_back(x);
            return;
        }
        for(int j = static_cast<int>(v[i].second);; --j) {
            rc(rc, i + 1, x);
            if(j == 0)
                break;
            x *= v[i].first;
        }
    };

    f(f, 0, 1);
    std::ranges::sort(res);

    return res;
}


} // namespace fast_factorize_impl

} // namespace internal


using internal::fast_factorize_impl::divisors;
using internal::fast_factorize_impl::factorize;
using internal::fast_factorize_impl::count_factors;
using internal::fast_factorize_impl::prime_factors;
using internal::fast_factorize_impl::is_prime;


} // namespace lib

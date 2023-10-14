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


#include "snippet/internal/types.hpp"

#include "internal/dev_env.hpp"
#include "internal/constants.hpp"

#include "numeric/modint.hpp"
#include "random/xorshift.hpp"

#include "adapter/set.hpp"
#include "adapter/map.hpp"
#include "adapter/vector.hpp"


namespace lib {

namespace internal {

//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {

namespace internal {


// Miller-Rabin primality test
template<typename mint> bool primality_test(const u64 n, const std::initializer_list<u64> as) noexcept(NO_EXCEPT) {
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wconversion"
    if(static_cast<u64>(mint::mod()) != n) mint::set_mod(n);
  #pragma GCC diagnostic pop

    u64 d = n-1;

    while(~d & 1) d >>= 1;

    mint e(1), rev(n - 1);
    for(u64 a : as) {
        if(n <= a) break;
        u64 t = d;
        mint y = mint(a).pow(t);
        while(t != n - 1 && y != e && y != rev) y *= y, t *= 2;
        if(y != rev && t % 2 == 0) return false;
    }

    return true;
}

bool is_prime(const u64 n) noexcept(NO_EXCEPT) {
    if(~n & 1) return n == 2;
    if(n <= 1) return false;

    if(n < (1UL << 31)) return internal::primality_test<dynamic_modint<lib::internal::INTERNAL_MODINT_ID_FOR_FAST_PRIME>>(n, { 2, 7, 61 });
    else return internal::primality_test<dynamic_modint_64bit<lib::internal::INTERNAL_MODINT_ID_FOR_FAST_PRIME>>(n, { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 });
}


// Pollard's rho algorithm
template<class mint, class T> T find_factor(const T n) noexcept(NO_EXCEPT) {
    if(~n & 1) return 2;
    if(is_prime(n)) return n;

    if(static_cast<u64>(mint::mod()) != n) mint::set_mod(n);

    mint R, one = 1;

    auto f = [&](mint x) noexcept(NO_EXCEPT) { return x * x + R; };

    static xorshift64 rand(std::random_device{}());
    auto rand_ = [&]() noexcept(NO_EXCEPT) { return rand() % (n - 2) + 2; };

    while(true) {

        mint x, y, ys, q = one;
        R = rand_(), y = rand_();
        T g = 1;
        constexpr int m = 128;

        for(int r = 1; g == 1; r <<= 1) {
            x = y;
            for(int i = 0; i < r; ++i)
                y = f(y);
            for(int k = 0; g == 1 && k < r; k += m) {
                ys = y;
                for(int i = 0; i < m && i < r - k; ++i)
                    q *= x - (y = f(y));
                g = std::gcd(q.val(), n);
            }
        }

        if(g == n) {
            do {
                g = std::gcd((x - (ys = f(ys))).val(), n);
            } while(g == 1);
        }
        if(g != n) return g;
    }

    assert(false);
}


vector<i64> factorize(const u64 n) noexcept(NO_EXCEPT) {
    if(n <= 1) return {};

    u64 p;
    if(n <= (1UL << 31)) p = find_factor<modint>(static_cast<u32>(n));
    else p = find_factor<modint64,u64>(n);

    if(p == n) return { static_cast<i64>(p) };

    auto l = internal::factorize(p);
    auto r = internal::factorize(n / p);

    std::copy(std::begin(r), std::end(r), std::back_inserter(l));

    return l;
}


} // namespace internal


inline i64 is_prime(const i64 n) noexcept(NO_EXCEPT) {
    return internal::is_prime(static_cast<u64>(n));
}

inline vector<i64> factorize(const u64 n) noexcept(NO_EXCEPT) {
    auto res = internal::factorize(n);
    std::sort(std::begin(res), std::end(res));
    return res;
}

inline set<i64> prime_factors(const u64 n) noexcept(NO_EXCEPT) {
    auto factors = factorize(n);
    set<i64> res(std::begin(factors), std::end(factors));
    return res;
}

inline map<i64,i64> count_factors(const u64 n) noexcept(NO_EXCEPT) {
    map<i64,i64> mp;
    for(auto &x : internal::factorize(n)) mp[x]++;
    return mp;
}

inline vector<i64> divisors(const u64 n) noexcept(NO_EXCEPT) {
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
    std::sort(std::begin(res), std::end(res));

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

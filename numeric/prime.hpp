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

#include "internal/dev_env.hpp"

#include "numeric/modint.hpp"
#include "random/xorshift.hpp"


namespace lib {

namespace internal {

//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {


using value_type = uint64_t;


namespace internal {


// Miller-Rabin primality test
template<typename mint> bool primality_test(const value_type n, const std::initializer_list<value_type> as) noexcept(NO_EXCEPT) {
    if(static_cast<value_type>(mint::mod()) != n) mint::set_mod(n);

    value_type d = n-1;

    while(~d & 1) d >>= 1;

    mint e(1), rev(n - 1);
    for(value_type a : as) {
        if(n <= a) break;
        value_type t = d;
        mint y = mint(a).pow(t);
        while(t != n - 1 && y != e && y != rev) y *= y, t *= 2;
        if(y != rev && t % 2 == 0) return false;
    }

    return true;
}


} // namespace internal


bool is_prime(const value_type n) noexcept(NO_EXCEPT) {
    if(~n & 1) return n == 2;
    if(n <= 1) return false;

    if(n < (1UL << 31)) return internal::primality_test<modint>(n, { 2, 7, 61 });
    else return internal::primality_test<modint64>(n, { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 });
}

using int64_t = std::int64_t;

namespace internal {


// Pollard's rho algorithm
template <typename mint, typename T> T find_factor(const T n) noexcept(NO_EXCEPT) {
    if(~n & 1) return 2;
    if(is_prime(n)) return n;

    if(static_cast<value_type>(mint::mod()) != n) mint::set_mod(n);

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


std::vector<int64_t> factorize(const value_type n) noexcept(NO_EXCEPT) {
    if(n <= 1) return {};

    value_type p;
    if(n <= (1UL << 31)) p = find_factor<modint,std::uint32_t>(n);
    else p = find_factor<modint64,std::uint64_t>(n);

    if(p == n) return { int64_t(p) };

    auto l = internal::factorize(p);
    auto r = internal::factorize(n / p);

    std::copy(std::begin(r), std::end(r), std::back_inserter(l));

    return l;
}


} // namespace internal


std::vector<int64_t> factorize(const value_type n) noexcept(NO_EXCEPT) {
    auto res = internal::factorize(n);
    std::sort(std::begin(res), std::end(res));
    return res;
}

std::set<int64_t> prime_factors(const value_type n) noexcept(NO_EXCEPT) {
    auto factors = factorize(n);
    std::set<int64_t> res(std::begin(factors), std::end(factors));
    return res;
}

std::map<int64_t,int64_t> count_factors(const value_type n) noexcept(NO_EXCEPT) {
    std::map<int64_t,int64_t> mp;
    for(auto &x : internal::factorize(n)) mp[x]++;
    return mp;
}

std::vector<int64_t> divisors(const value_type n) noexcept(NO_EXCEPT) {
    if(n == 0) return {};

    std::vector<std::pair<int64_t, int64_t>> v;
    for(auto &p : factorize(n)) {
        if(v.empty() || v.back().first != p) {
            v.emplace_back(p, 1);
        } else {
            v.back().second++;
        }
    }

    std::vector<int64_t> res;
    auto f = [&](auto rc, int i, int64_t x) noexcept(NO_EXCEPT) -> void {
        if(i == (int)v.size()) {
            res.push_back(x);
            return;
        }
        for(int j = v[i].second;; --j) {
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

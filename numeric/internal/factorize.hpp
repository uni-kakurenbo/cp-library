#pragma once


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
#include "numeric/internal/modint_interface.hpp"

#include "random/xorshift.hpp"

#include "adapter/set.hpp"
#include "adapter/map.hpp"
#include "adapter/vector.hpp"


namespace lib {

namespace internal {


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {

namespace internal {


// Pollard's rho algorithm
template<dynamic_modint_family Mint, class T>
T find_factor(const T n) noexcept(NO_EXCEPT) {
    if(~n & 1) return 2;
    if(is_prime<Mint>(n)) return n;

    assert(static_cast<u64>(Mint::mod()) == n);
    Mint rr, one = 1;

    auto f = [&](const Mint& x) noexcept(NO_EXCEPT) { return x * x + rr; };

    static xorshift64 rand(std::random_device{}());
    auto rand_ = [&]() noexcept(NO_EXCEPT) { return rand() % (n - 2) + 2; };

    while(true) {
        Mint x, y, ys, q = one;
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


template<modint_family Small, modint_family Large>
vector<i64> factorize(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    if(n <= 1) return {};

    u64 p;
    if(n <= Small::max()) p = find_factor<Small>(static_cast<u32>(n));
    else p = find_factor<Large, u64>(n);

    if(p == static_cast<u64>(n)) return { static_cast<i64>(p) };

    auto l = factorize<Small, Large>(p);
    auto r = factorize<Small, Large>(n / p);

    std::ranges::copy(r, std::back_inserter(l));

    return l;
}


} // namespace internal

} // namespace fast_factorize_impl


using fast_factorize::internal::factorize;


} // namespace internal

} // namespace lib

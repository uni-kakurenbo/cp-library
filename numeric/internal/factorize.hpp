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
#include "numeric/modular/modint_interface.hpp"
#include "numeric/bit.hpp"

#include "random/xorshift.hpp"


namespace lib {

namespace internal {


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {

namespace internal {


// Pollard's rho algorithm
template<modint_family Mint, class T>
T find_factor(const T n) noexcept(NO_EXCEPT) {
    if(~n & 1) return 2;
    if(is_prime<Mint>(n)) return n;

    assert(static_cast<u64>(Mint::mod()) == n);
    Mint rr;

    auto f = [&](const Mint& x) noexcept(NO_EXCEPT) { return x * x + rr; };

    static xorshift64<-(1L << 62) + 2> rand(std::random_device{}());
    auto rand_ = [&]() noexcept(NO_EXCEPT) { return rand() % (n - 2) + 2; };

    while(true) {
        Mint x, y, ys, q = Mint::one();
        rr = rand_(), y = rand_();
        T g = 1;
        constexpr int m = 128;

        for(int r = 1; g == 1; r <<= 1) {
            x = y;
            for(int i = 0; i < r; ++i) y = f(y);
            for(int k = 0; g == 1 && k < r; k += m) {
                ys = y;
                for(int i = 0; i < m && i < r - k; ++i) q *= x - (y = f(y));
                g = lib::binary_gcd(q.val(), n);
            }
        }

        if(g == n) {
            do {
                g = lib::binary_gcd((x - (ys = f(ys))).val(), n);
            } while(g == 1);
        }
        if(g != n) return g;
    }

    assert(false);
}


template<modint_family Small, modint_family Large, std::ranges::range R>
void factorize(const u64 n, R *const res) noexcept(NO_EXCEPT) {
    if(n <= 1) return;

    u64 p;
    if constexpr(std::same_as<Small, Large>) p = find_factor<Small, typename Small::value_type>(n);
    else {
        if(n <= Small::max()) p = find_factor<Small, typename Small::value_type>(n);
        else p = find_factor<Large, typename Large::value_type>(n);
    }

    if(p == static_cast<u64>(n)) {
        res->emplace_back(p);
        return;
    }

    factorize<Small, Large>(p, res);
    factorize<Small, Large>(n / p, res);
}


} // namespace internal

} // namespace fast_factorize_impl


using fast_factorize_impl::internal::factorize;


} // namespace internal

} // namespace lib

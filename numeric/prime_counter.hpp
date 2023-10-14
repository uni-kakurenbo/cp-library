#pragma once


#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"

#include "numeric/arithmetic.hpp"


namespace lib {


static inline i64 float_div(const i64 n, const i64 p) {
    return static_cast<i64>(static_cast<double>(n) / p);
};

// Thanks to: https://nyaannyaan.github.io/library/multiplicative-function/prime-counting-faster.hpp
__attribute__((target("avx2"), optimize("O3", "unroll-loops")))
i64 count_primes(const i64 n) noexcept(NO_EXCEPT) {
    if(n <= 1) return n;

    const i64 sqrt_n = lib::sqrt_floor(n);
    const i64 m = float_div(n, sqrt_n);

    std::valarray<i64> hl(m);
    REP(i, 1, m) hl[i] = float_div(n, i) - 1;

    std::valarray<i32> hs(sqrt_n + 1);
    std::iota(ALL(hs), -1);

    for(i32 x=2, pi=0; x <= sqrt_n; ++x) {
        if(hs[x] == hs[x - 1]) continue;
        const i64 x2 = static_cast<i64>(x) * x;
        const i64 imax = std::min(m, float_div(n, x2) + 1);

        i64 ix = x;
        REP(i, 1, imax) {
            hl[i] -= (ix < m ? hl[ix] : hs[float_div(n, ix)]) - pi;
            ix += x;
        }
        FORD(k, x2, sqrt_n) hs[k] -= hs[float_div(k, x)] - pi;

        ++pi;
    }

    return hl[1];
}


inline i64 count_primes(const i64 l, const i64 r) noexcept(NO_EXCEPT) {
    assert(l <= r);
    return count_primes(r) - count_primes(l - 1);
}


} // namespace lib

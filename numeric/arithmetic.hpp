#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include <algorithm>
#include <atcoder/math.hpp>

#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/internal/number_base.hpp"

namespace lib {


template<class T>
T sqrt_floor(const T x) noexcept(NO_EXCEPT) {
    T ok = 0, ng = x / 2 + 2;
    while(ng - ok > 1) {
        T mid = (ok + ng) / 2;
        (x / mid < mid ? ng : ok) = mid;
    }
    return ok;
}

template<class T>
T sqrt_ceil(const T x) noexcept(NO_EXCEPT) {
    T ok = 0, ng = x / 2 + 2;
    while(ng - ok > 1) {
        T mid = (ok + ng) / 2;
        (x <= (mid - 1) * (mid - 1) ? ng : ok) = mid;
    }
    return ok;
}

template<class T>
T nPr(const T n, const T r) noexcept(NO_EXCEPT) {
    assert(0 <= n);
    assert(0 <= r);
    if(n < r) return 0;

    T res = 1;
    REP(i, r) res *= n-i;

    return res;
}

template<class T>
T nCr(const T n, const T r) noexcept(NO_EXCEPT) {
    assert(0 <= n);
    assert(0 <= r);
    if(n == r) return 1;
    if(n < r) return 0;

    if(n < r*2) r = n-r;
    T p = 1, q = 1;
    REP(i, r) p *= n-i, q *= r-i;

    return p / q;
}


template<class T, class U>
T pow(T x, U n) noexcept(NO_EXCEPT) {
    T res = 1;
    while(n > 0) {
        if(n & 1) res *= x;
        x *= x;
        n >>= 1;
    }
    return res;
}

using atcoder::pow_mod;
using atcoder::inv_mod;
using atcoder::crt;

template<class T, class U, class V>inline bool mul_over(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    if(x >= s) return true;
    if(x >= 0 && y >= 0) return x > s/y;
    if(x <= 0 && y <= 0) return x < s/y;
    return false;
}

template<class T, class U, class V>inline bool mul_under(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    if(x <= s) return true;
    if(x >= 0 && y <= 0) return x > s/y;
    if(x <= 0 && y >= 0) return x < s/y;
    return false;
}


} // namespace lib

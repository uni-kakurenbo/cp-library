#pragma once

#include <cstring>
#include <string>
#include <algorithm>
#include <atcoder/math.hpp>

#include "internal/types.hpp"
#include "snippet/aliases.hpp"

#include "numeric/internal/number_base.hpp"

namespace lib {


template<class T>
T nPr(const T n, T r) {
    assert(0 <= n);
    assert(0 <= r);
    if(n < r) return 0;

    T res = 1;
    REP(i, r) res *= n-i;

    return res;
}

template<class T>
T nCr(const T n, T r) {
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
T pow(T x, U n) {
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

template<class T, class U, class V>inline bool mul_over(T x, U y, V s){
    if(x >= s) return true;
    if(x >= 0 && y >= 0) return x > s/y;
    if(x <= 0 && y <= 0) return x < s/y;
    return false;
}

template<class T, class U, class V>inline bool mul_under(T x, U y, V s){
    if(x <= s) return true;
    if(x >= 0 && y <= 0) return x > s/y;
    if(x <= 0 && y >= 0) return x < s/y;
    return false;
}


} // namespace lib

#pragma once


#include <cassert>
#include <cstring>
#include <type_traits>
#include <string>
#include <utility>
#include <optional>
#include <algorithm>
#include <atcoder/math.hpp>

#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/internal/number_base.hpp"


namespace lib {


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


template<class T, class U, std::enable_if_t<(std::is_integral_v<T> and std::is_integral_v<U>)>* = nullptr>
std::optional<std::common_type_t<T,U>> add_overflow(const T& a, const U& b) {
    std::common_type_t<T,U> res;
    if(__builtin_add_overflow(a, b, &res)) return {};
    else return res;
}

template<class T, class U, std::enable_if_t<(std::is_integral_v<T> and std::is_integral_v<U>)>* = nullptr>
std::optional<std::common_type_t<T,U>> sub_overflow(const T& a, const U& b) {
    std::common_type_t<T,U> res;
    if(__builtin_sub_overflow(a, b, &res)) return {};
    else return res;
}

template<class T, class U, std::enable_if_t<(std::is_integral_v<T> and std::is_integral_v<U>)>* = nullptr>
std::optional<std::common_type_t<T,U>> mul_overflow(const T& a, const U& b) {
    std::common_type_t<T,U> res;
    if(__builtin_mul_overflow(a, b, &res)) return {};
    else return res;
}


template<class T, class U, class V>
inline bool mul_over(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    using Com = std::common_type_t<T,U,V>;
    const auto res = mul_overflow<Com,Com>(x, y);
    if(!res) return !((x < 0) xor (y < 0));
    return *res > s;
}

template<class T, class U, class V>
inline bool mul_or_over(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    using Com = std::common_type_t<T,U,V>;
    const auto res = mul_overflow<Com,Com>(x, y);
    if(!res) return !((x < 0) xor (y < 0));
    return *res >= s;
}

template<class T, class U, class V>
inline bool mul_under(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    using Com = std::common_type_t<T,U,V>;
    const auto res = mul_overflow<Com,Com>(x, y);
    if(!res) return ((x < 0) xor (y < 0));
    return *res < s;
}

template<class T, class U, class V>
inline bool mul_or_under(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    using Com = std::common_type_t<T,U,V>;
    const auto res = mul_overflow<Com,Com>(x, y);
    if(!res) return ((x < 0) xor (y < 0));
    return *res <= s;
}

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
        (mul_over(mid-1, mid-1, x-1) ? ng : ok) = mid;
    }
    return ok;
}


} // namespace lib

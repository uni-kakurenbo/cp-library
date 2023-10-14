#pragma once


#include <cassert>
#include <cstring>
#include <type_traits>
#include <string>
#include <utility>
#include <optional>
#include <algorithm>
#include <atcoder/math.hpp>
#include <concepts>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/internal/number_base.hpp"

#include "iterable/operation.hpp"


namespace lib {


template<class T>
inline constexpr T div_ceil(const T& x, const T& d) noexcept(NO_EXCEPT) {
    return x / d + static_cast<T>(x % d);;
}

template<class T>
inline constexpr std::make_signed_t<T> to_signed(const T& x) noexcept(NO_EXCEPT) {
    return static_cast<std::make_signed_t<T>>(x);
}

template<class T>
inline constexpr std::make_unsigned_t<T> to_unsigned(const T& x) noexcept(NO_EXCEPT) {
    return static_cast<std::make_unsigned_t<T>>(x);
}


template<class T, class R = T>
inline constexpr R nPr(const T& n, const T& r) noexcept(NO_EXCEPT) {
    assert(0 <= n);
    assert(0 <= r);
    if(n < r) return 0;

    R res = 1;
    REP(i, r) res *= n-i;

    return res;
}

template<class T, class R = T>
inline constexpr R nCr(const T& n, T r) noexcept(NO_EXCEPT) {
    assert(0 <= n);
    assert(0 <= r);
    if(n == r) return 1;
    if(n < r) return 0;

    if(n < r*2) r = n-r;
    R p = 1, q = 1;
    REP(i, r) p *= n-i, q *= r-i;

    return p / q;
}


template<class T, class R = T>
inline constexpr R factorial(const T& n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    R ans = 1;
    FOR(k, 1, n) ans *= k;
    return ans;
}


template<class T, class U>
inline constexpr auto pow(const T& x, U n) noexcept(NO_EXCEPT) -> decltype(x.pow(n), T()) { return x.pow(n); }

template<class T, class U>
inline constexpr auto pow(T x, U n) noexcept(NO_EXCEPT) -> T {
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


template<class T> inline constexpr T sign(const T& x) noexcept(NO_EXCEPT) {
    if(x == 0) return 0;
    return (x > 0) ? 1 : -1;
}

template<class T, T FROM_MIN, T FROM_MAX, T TO_MIN, T TO_MAX> inline constexpr T mapping(const T x) {
    return (x - FROM_MIN) * (TO_MAX - TO_MIN) / (FROM_MAX - FROM_MIN) + TO_MIN;
}
template<class T> inline constexpr T mapping(const T x, const T from_min, const T from_max, const T to_min, const T to_max) {
    return (x - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}

template<class... Args>
std::common_type_t<Args...> min(const Args&... args) noexcept(NO_EXCEPT) {
    return std::min({ static_cast<std::common_type_t<Args...>>(args)... });
}

template<class... Args>
inline constexpr std::common_type_t<Args...> max(const Args&... args) noexcept(NO_EXCEPT) {
    return std::max({ static_cast<std::common_type_t<Args...>>(args)... });
}


template<class T>
inline constexpr T gcd(const std::initializer_list<T> args) noexcept(NO_EXCEPT) {
    return gcd(ALL(args));
}

template<class... Args>
inline constexpr std::common_type_t<Args...> gcd(const Args&... args) noexcept(NO_EXCEPT) {
    return gcd({ static_cast<std::common_type_t<Args...>>(args)... });
}


template<class T>
inline constexpr T lcm(const std::initializer_list<T> args) noexcept(NO_EXCEPT) {
    return lcm(ALL(args));
}

template<class... Args>
inline constexpr std::common_type_t<Args...> lcm(const Args&... args) noexcept(NO_EXCEPT) {
    return lcm({ static_cast<std::common_type_t<Args...>>(args)... });
}


template<std::integral T, std::integral U>
inline constexpr std::optional<std::common_type_t<T,U>> add_overflow(const T& a, const U& b) noexcept(NO_EXCEPT) {
    std::common_type_t<T,U> res;
    if(__builtin_add_overflow(a, b, &res)) return {};
    else return res;
}

template<std::integral T, std::integral U>
inline constexpr std::optional<std::common_type_t<T,U>> sub_overflow(const T& a, const U& b) noexcept(NO_EXCEPT) {
    std::common_type_t<T,U> res;
    if(__builtin_sub_overflow(a, b, &res)) return {};
    else return res;
}

template<std::integral T, std::integral U>
inline constexpr std::optional<std::common_type_t<T,U>> mul_overflow(const T& a, const U& b) noexcept(NO_EXCEPT) {
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
inline constexpr T sqrt_floor(const T x) noexcept(NO_EXCEPT) {
    T res = static_cast<T>(std::sqrt(x));

    if constexpr(std::is_floating_point_v<T>) {
        while((res + 1) * (res + 1) <= x) res += 1;
    }
    else {
        while(mul_overflow(res + 1, res + 1).value_or(std::numeric_limits<T>::max()) <= x) ++res;
    }

    return res;
}

template<class T>
inline constexpr T sqrt_ceil(const T x) noexcept(NO_EXCEPT) {
    T res = static_cast<T>(std::sqrt(x));

    if constexpr(std::is_floating_point_v<T>) {
        while(res * res < x) res += 1;
    }
    else {
        while(mul_overflow(res, res).value_or(std::numeric_limits<T>::max()) < x) ++res;
    }

    return res;
}


} // namespace lib

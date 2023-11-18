#pragma once


#include <cassert>
#include <cstring>
#include <type_traits>
#include <string>
#include <utility>
#include <optional>
#include <algorithm>
#include <atcoder/math>
#include <concepts>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/internal/number_base.hpp"
#include "numeric/modint.hpp"

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
    requires lib::internal::is_modint_v<T>
inline constexpr T pow(const T& x, U n) noexcept(NO_EXCEPT) { return x.pow(n); }

template<class T, class U>
    requires (not lib::internal::is_modint_v<T>)
inline constexpr T pow(T x, U n) noexcept(NO_EXCEPT) {
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
inline constexpr std::common_type_t<Args...> min(const Args&... args) noexcept(NO_EXCEPT) {
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


template<std::integral T0, std::integral T1>
inline constexpr std::optional<std::common_type_t<T0, T1>> add_overflow(const T0& a, const T1& b) noexcept(NO_EXCEPT) {
    std::common_type_t<T0, T1> res;
    if(__builtin_add_overflow(a, b, &res)) return {};
    return res;
}

template<std::integral T0, std::integral T1>
inline constexpr std::optional<std::common_type_t<T0, T1>> sub_overflow(const T0& a, const T1& b) noexcept(NO_EXCEPT) {
    std::common_type_t<T0, T1> res;
    if(__builtin_sub_overflow(a, b, &res)) return {};
    return res;
}

template<std::integral T0, std::integral T1>
inline constexpr std::optional<std::common_type_t<T0, T1>> mul_overflow(const T0& a, const T1& b) noexcept(NO_EXCEPT) {
    std::common_type_t<T0, T1> res;
    if(__builtin_mul_overflow(a, b, &res)) return {};
    return res;
}

template<std::integral T0, std::integral T1, std::integral Limit>
inline auto add_clamp(const T0 x, const T1 y, const Limit inf, const Limit sup) noexcept(NO_EXCEPT) {
    using Common = std::common_type_t<T0, T1, Limit>;
    const auto res = add_overflow<Common>(x, y);
    if(!res) {
        if(x < 0 && y < 0) return inf;
        if(x > 0 && y > 0) return sup;
        assert(false);
    }
    return std::clamp<Common>(*res, inf, sup);
}

template<std::integral T0, std::integral T1, std::integral Limit>
inline auto sub_clamp(const T0 x, const T1 y, const Limit inf, const Limit sup) noexcept(NO_EXCEPT) {
    using Common = std::common_type_t<T0, T1, Limit>;
    const auto res = sub_overflow<Common>(x, y);
    if(!res) {
        if(x < 0 && y > 0) return inf;
        if(x > 0 && y < 0) return sup;
        assert(false);
    }
    return std::clamp<Common>(*res, inf, sup);
}

template<std::integral T0, std::integral T1, std::integral Limit>
inline auto mul_clamp(const T0 x, const T1 y, const Limit inf, const Limit sup) noexcept(NO_EXCEPT) {
    using Common = std::common_type_t<T0, T1, Limit>;
    const auto res = mul_overflow<Common>(x, y);
    if(!res) {
        if((x > 0) xor (y > 0)) return inf;
        else return sup;
        assert(false);
    }
    return std::clamp<Common>(*res, inf, sup);
}


template<class T>
inline constexpr T sqrt_floor(const T x) noexcept(NO_EXCEPT) {
    return static_cast<T>(std::sqrt(static_cast<long double>(x)));
}

template<class T>
inline constexpr T sqrt_ceil(const T x) noexcept(NO_EXCEPT) {
    T res = sqrt_floor(x);

    if constexpr(std::is_floating_point_v<T>) {
        while(res * res < x) res += 1;
    }
    else {
        while(mul_overflow(res, res).value_or(std::numeric_limits<T>::max()) < x) ++res;
    }

    return res;
}

template<class T>
inline constexpr T kth_root_floor(T x, const i64 k) noexcept(NO_EXCEPT) {
    assert(x >= 0), assert(k > 0);
    if(x <= 1 or k == 1) return x;

    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(k >= DIGITS) return T{1};
    if(k == 2) return sqrt_floor(x);

    constexpr T MAX = std::numeric_limits<T>::max();
    if(x == MAX) --x;

    auto pow = [&](T t, i64 p) {
        if(p == 0) return T{1};
        T res = 1;
        while(p) {
            if(p & 1) {
                T _res;
                if(__builtin_mul_overflow(res, t, &_res)) res = MAX;
                else res = _res;
            }
            T _x;
            if(__builtin_mul_overflow(t, t, &_x)) t = MAX;
            else t = _x;
            p >>= 1;
        }
        return res;
    };

    T res = std::pow(x, std::nextafter(1 / static_cast<double>(k), 0));
    while(pow(res + 1, k) <= x) ++res;

    return res;
}


} // namespace lib

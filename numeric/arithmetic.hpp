#pragma once


#include <cassert>
#include <cstring>
#include <type_traits>
#include <string>
#include <functional>
#include <utility>
#include <optional>
#include <algorithm>
#include <concepts>
#include <bit>


#include <atcoder/math>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"

#include "utility/internal/functional_base.hpp"

#include "numeric/internal/number_base.hpp"

#include "iterable/operation.hpp"


namespace uni {


template<class T>
inline constexpr T div_floor(const T& x, const T& d) noexcept(NO_EXCEPT) {
    if constexpr(std::is_integral_v<T>) {
        return x / d - (x % d && ((x < 0) ^ (d < 0)));
    }
    else {
        return std::floor(x / d);
    }
}

template<class T>
inline constexpr T div_ceil(const T& x, const T& d) noexcept(NO_EXCEPT) {
    if constexpr(std::is_integral_v<T>) {
        return div_floor(x + d - 1, d);
    }
    else {
        return std::ceil(x / d);
    }
}

template<class T>
inline constexpr T div_round(const T& x, const T& d) noexcept(NO_EXCEPT) {
    if constexpr(std::is_integral_v<T>) {
        return div_round<ld>(x, d);
    }
    else {
        return std::round(x / d);
    }
}


template<class T>
inline constexpr std::make_signed_t<T> to_signed(const T& x) noexcept(NO_EXCEPT) {
    return std::bit_cast<std::make_signed_t<T>>(x);
}

template<class T>
inline constexpr std::make_unsigned_t<T> to_unsigned(const T& x) noexcept(NO_EXCEPT) {
    return std::bit_cast<std::make_unsigned_t<T>>(x);
}


namespace internal {


template<class T>
inline constexpr auto perm(const T& n, const T& r) noexcept(NO_EXCEPT) {
    T res = 1;
    REP(i, r) res *= n - i;
    return res;
}

template<class T>
inline constexpr auto comb(const T& n, T r) noexcept(NO_EXCEPT) {
    if(n < 2 * r) r = n - r;

    T p = 1, q = 1;
    REP(i, r) p *= n - i, q *= r - i;

    return p / q;
}


} // namespace internal


template<class T0, std::common_with<T0> T1>
inline constexpr auto perm(const T0& n, const T1& r) noexcept(NO_EXCEPT) {
    assert(n >= 0), assert(r >= 0);
    using T = std::common_type_t<T0, T1>;
    if(n < r) return static_cast<T>(0);
    return internal::perm<T>(n, r);
}

template<class T0, std::common_with<T0> T1>
inline constexpr auto comb(const T0& n, const T1& r) noexcept(NO_EXCEPT) {
    assert(n >= 0), assert(r >= 0);
    using T = std::common_type_t<T0, T1>;
    if(n < r) return static_cast<T>(0);
    if(n == r) return static_cast<T>(1);
    return internal::comb<T>(n, r);
}



template<class T, class U, std::invocable<T, T> F = std::multiplies<>>
constexpr T pow(T x, U n, F mul = F(), T one = static_cast<T>(1)) noexcept(NO_EXCEPT) {
    if(n == 0) return one;
    if(n == 1 || x == one) return x;

    T res = one;

    while(true) {
        if(n & 1) res = mul(res, x);
        x = mul(x, x);
        if(n == 0) return res;
        n >>= 1;
    }

    assert(false);
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

template<class T, std::integral K>
inline constexpr T kth_root_floor(T x, const K k) noexcept(NO_EXCEPT) {
    assert(x >= 0);
    if(std::signed_integral<K>) assert(k > 0);

    if(x <= 1 or k == 1) return x;

    constexpr auto DIGITS = std::numeric_limits<T>::digits;
    if(k >= DIGITS) return T{1};
    if(k == 2) return sqrt_floor(x);

    constexpr auto MAX = std::numeric_limits<T>::max();
    if(x == MAX) --x;

    auto pow = [&](T t, i64 p) {
        if(p == 0) return T{1};
        T res = 1;
        while(p) {
            if(p & 1) {
                res = mul_overflow(res, t).value_or(MAX);
            }
            t = mul_overflow(t, t).value_or(MAX);
            p >>= 1;
        }
        return res;
    };

    auto res = static_cast<T>(std::pow(x, std::nextafter(1 / static_cast<double>(k), 0)));
    while(pow(res + 1, k) <= x) ++res;

    return res;
}


template<std::integral T>
T inline constexpr extended_gcd(const T& a, const T& b, T& x, T& y) noexcept {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    const T d = extended_gcd(b, a%b, y, x);

    y -= a / b * x;
    return d;
};

template<std::integral T>
std::pair<T, spair<T>> inline constexpr extended_gcd(const T& a, const T& b) noexcept {
    T x, y;
    const T d = extended_gcd(a, b, x, y);
    return { d, spair<T>{ x, y } };
};


template<std::integral T>
std::optional<spair<T>> inline constexpr bezout_equation(const T& a, const T& b, const T& c) noexcept {
    if(a == 0) {
        if(b == 0) {
            if(c == 0) return spair<T>{ 0, 0 };
            else { };
        }
        if(c % b == 0) return spair<T>{ 0, c / b };
        return {};
    }

    if(b == 0) {
        const auto ans = bezout_equation(b, a, c);
        if(ans.has_value()) return swapped(ans.value());
        return {};
    }

    T x, y;
    const T gcd = extended_gcd(a, b, x, y);

    if(c % gcd != 0) return {};

    const T p = c / gcd;
    return spair<T>{ x * p, y * p };
};


} // namespace uni

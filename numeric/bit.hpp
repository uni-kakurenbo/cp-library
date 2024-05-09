#pragma once


#include <immintrin.h>

#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <concepts>
#include <compare>
#include <bit>


#include "internal/dev_env.hpp"

#include "numeric/arithmetic.hpp"


namespace uni {


template<std::unsigned_integral T>
constexpr T multiply_high(const T x, const T y) noexcept(NO_EXCEPT) {
    constexpr int digits = std::numeric_limits<T>::digits;

    if constexpr(digits <= 16) {
        return static_cast<T>((static_cast<u32>(x) * static_cast<u32>(y)) >> digits);
    }
    else if constexpr(digits <= 32) {
        return static_cast<T>((static_cast<u64>(x) * static_cast<u64>(y)) >> digits);
    }
    else if constexpr(digits <= 64) {
        return static_cast<T>((static_cast<u128>(x) * static_cast<u128>(y)) >> digits);
    }
    else {
        constexpr int h_digits = digits / 2;
        constexpr T mask = (T{ 1 } << h_digits) - 1;

        const T xh = x >> h_digits, yh = y >> h_digits;
        const T xl = x & mask, yl = y & mask;
        const T ph = xh * yh, pl = xl * yl;

        return (((pl >> h_digits) + (xh + xl) * (yh + yl) - (ph + pl)) >> h_digits) + ph;
    }
}


template<std::unsigned_integral T>
inline constexpr int highest_bit_pos(const T v) noexcept(NO_EXCEPT) {
    return (int)std::bit_width(v) - 1; // cast to int for GCC12
}

template<std::unsigned_integral T>
inline constexpr int lowest_bit_pos(const T v) noexcept(NO_EXCEPT) {
    if(v == 0) return -1;
    return std::countr_zero(v);
}


template<std::unsigned_integral T, std::integral I = int>
__attribute__((target("bmi2")))
inline constexpr T clear_higher_bits(const T v, const I p) {
    if constexpr(std::signed_integral<I>) assert(0 <= p);

    constexpr int DIGITS = std::numeric_limits<T>::digits;
    assert(p <= DIGITS);

    if constexpr(DIGITS <= 32) return _bzhi_u32(v, static_cast<u32>(p));
    if constexpr(DIGITS <= 64) return _bzhi_u64(v, static_cast<u64>(p));
    else {
        static_assert(DIGITS <= 128);

        constexpr std::uint64_t MAX64 = std::numeric_limits<std::uint64_t>::max();

        const std::uint64_t high = v >> 64;
        const std::uint64_t low = v & MAX64;

        if(p < 64) return _bzhi_u64(low, p);
        return low | (T{_bzhi_u64(high, p - 64)} << 64);
    }
}


template<std::unsigned_integral T, std::integral I = int> constexpr T shiftl(const T, const I = 1);
template<std::unsigned_integral T, std::integral I = int> constexpr T shiftr(const T, const I = 1);

template<std::unsigned_integral T, std::integral I>
constexpr T shiftl(const T x, const I n) {
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if constexpr(std::signed_integral<I>) {
        if(n < 0) return shiftr(x, -n);
    }
    if(n >= DIGITS) return 0;
    return x << n;
}

template<std::unsigned_integral T, std::integral I>
constexpr T shiftr(const T x, const I n) {
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if constexpr(std::signed_integral<I>) {
        if(n < 0) return shiftl(x, -n);
    }
    if(n >= DIGITS) return 0;
    return x >> n;
}


template<std::unsigned_integral T, std::integral I = int>
inline constexpr bool bit(const T x, const I p) {
    if constexpr(std::signed_integral<I>) assert(0 <= p);
    assert(p < std::numeric_limits<T>::digits);

    return shiftr(x, p) & T{1};
}


template<std::unsigned_integral T, std::integral I = int>
inline constexpr bool reset_bit(const T x, const I p) {
    if constexpr(std::signed_integral<I>) assert(0 <= p);
    assert(p < std::numeric_limits<T>::digits);

    return x & ~(T{1} << p);
}


template<std::unsigned_integral T, std::integral I = int>
inline constexpr bool set_bit(const T x, const I p, const bool bit = true) {
    if constexpr(std::signed_integral<I>) assert(0 <= p);
    assert(p < std::numeric_limits<T>::digits);

    if(!bit) return reset(x, p);
    return x | (T{1} << p);
}


template<std::unsigned_integral T, std::integral I = int>
inline constexpr T lower_bits(const T x, const I digits) {
    if constexpr(std::signed_integral<I>) assert(0 <= digits);
    assert(digits <= std::numeric_limits<T>::digits);

    return x & (uni::shiftl(x, digits) - 1);
}


// Thanks to: https://noshi91.github.io/Library/other/select64.cpp
constexpr int select64(const u64 x0, u32 k) {
    const u64 x1 = (x0 & UINT64_C(0x5555555555555555)) + (x0 >> 1 & UINT64_C(0x5555555555555555));
    const u64 x2 = (x1 & UINT64_C(0x3333333333333333)) + (x1 >> 2 & UINT64_C(0x3333333333333333));
    const u64 x3 = (x2 & UINT64_C(0x0F0F0F0F0F0F0F0F)) + (x2 >> 4 & UINT64_C(0x0F0F0F0F0F0F0F0F));
    const u64 x4 = (x3 & UINT64_C(0x00FF00FF00FF00FF)) + (x3 >> 8 & UINT64_C(0x00FF00FF00FF00FF));
    const u64 x5 = (x4 & UINT64_C(0x0000FFFF0000FFFF)) + (x4 >> 16 & UINT64_C(0x0000FFFF0000FFFF));

    int res = 0;

    u32 t;

    t = x5 & 0xFFFFFFFF;
    if(t <= k) k -= t, res += 32;

    t = x4 >> res & 0xFFFF;
    if(t <= k) k -= t, res += 16;

    t = x3 >> res & 0xFF;
    if(t <= k) k -= t, res += 8;

    t = x2 >> res & 0xF;
    if(t <= k) k -= t, res += 4;

    t = x1 >> res & 0x3;
    if(t <= k) k -= t, res += 2;

    t = x0 >> res & 0x1;
    if(t <= k) k -= t, res += 1;

    return res;
}


namespace internal {


template<std::unsigned_integral T>
constexpr T binary_gcd(T a, T b) noexcept(NO_EXCEPT) {
    if(!a || !b) return a | b;
    T t, s = std::countr_zero(a | b);
    a >>= std::countr_zero(a);
    do {
        b >>= std::countr_zero(b);
        if(a > b) t = a, a = b, b = t;
        b -= a;
    } while(b);
    return a << s;
}


template<std::signed_integral T>
inline constexpr T binary_gcd(const T a, const T b) noexcept(NO_EXCEPT) {
    return binary_gcd(a < 0 ? -a : a, b < 0 ? -b : b);
}


} // namespace internal


template<std::integral T0, std::integral T1>
inline constexpr auto binary_gcd(T0 v0, T1 v1) noexcept(NO_EXCEPT) {
    using common_type = std::common_type_t<T0, T1>;
    return internal::binary_gcd(static_cast<common_type>(v0), static_cast<common_type>(v1));
}


template<std::unsigned_integral T, std::unsigned_integral S>
inline constexpr bool is_subset_of(T target, S superset) noexcept(NO_EXCEPT) {
    return (target & ~superset) == 0;
}

template<std::unsigned_integral T, std::unsigned_integral S>
inline constexpr bool is_superset_of(T target, S subset) noexcept(NO_EXCEPT) {
    return (~target & subset) == 0;
}


template<std::unsigned_integral S0, std::unsigned_integral S1>
inline constexpr auto comapre_as_bitset(S0 s0, S1 s1) noexcept(NO_EXCEPT) {
    if(s0 == s1) return std::partial_ordering::equivalent;
    if(is_subset_of(s0, s1)) return std::partial_ordering::less;
    if(is_superset_of(s0, s1)) return std::partial_ordering::greater;
    return std::partial_ordering::unordered;
}


} // namespace uni

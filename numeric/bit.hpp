#pragma once


#include <immintrin.h>

#include <type_traits>
#include <cstdint>
#include <limits>

#include "internal/dev_env.hpp"


namespace lib {

#define LIB_STATUC_ASSERT_UNSIGNED(T) static_assert(std::is_unsigned_v<T>, "only unsigned type is supported")


template<class T>
__attribute__((target("bmi,bmi2,popcnt"))) inline constexpr int popcount(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);

    using u = unsigned int;
    using ul = unsigned long;
    using ull = unsigned long long;

    constexpr int DIGITS = std::numeric_limits<T>::digits;

    constexpr int DIGITS_U = std::numeric_limits<u>::digits;
    constexpr int DIGITS_UL = std::numeric_limits<ul>::digits;
    constexpr int DIGITS_ULL = std::numeric_limits<ull>::digits;

    if constexpr(DIGITS <= DIGITS_U) return __builtin_popcount(v);
    else if constexpr(DIGITS <= DIGITS_UL) return __builtin_popcountl(v);
    else if constexpr(DIGITS <= DIGITS_ULL) return __builtin_popcountll(v);
    else {
        static_assert(DIGITS <= DIGITS_ULL << 1);

        constexpr ull MAX_ULL = std::numeric_limits<ull>::max();

        const ull high = v >> DIGITS_ULL;
        const ull low = v & MAX_ULL;

        return __builtin_popcountll(high) + __builtin_popcountll(low);
    }
}

template<class T>
__attribute__((target("bmi,bmi2,lzcnt"))) inline constexpr int countl_zero(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);

    using u = unsigned int;
    using ul = unsigned long;
    using ull = unsigned long long;

    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(v == 0) return DIGITS;

    constexpr int DIGITS_U = std::numeric_limits<u>::digits;
    constexpr int DIGITS_UL = std::numeric_limits<ul>::digits;
    constexpr int DIGITS_ULL = std::numeric_limits<ull>::digits;

    if constexpr(DIGITS <= DIGITS_U) return __builtin_clz(v) - DIGITS_U + DIGITS;
    if constexpr(DIGITS <= DIGITS_UL) return __builtin_clzl(v) - DIGITS_UL + DIGITS;
    if constexpr(DIGITS <= DIGITS_ULL) return __builtin_clzll(v) - DIGITS_ULL + DIGITS;
    else {
        static_assert(DIGITS <= DIGITS_ULL << 1);

        constexpr ull MAX_ULL = std::numeric_limits<ull>::max();

        const ull high = v >> DIGITS_ULL;
        const ull low = v & MAX_ULL;

        if(high != 0) return __builtin_clzll(high) - (DIGITS_ULL << 1) + DIGITS;
        return  __builtin_clzll(low) - DIGITS_ULL + DIGITS;
    }
}

template<class T>
inline constexpr int bit_width(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    return std::numeric_limits<T>::digits - countl_zero(v);
}

template<class T>
inline constexpr int highest_bit_pos(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    return bit_width(v) - 1;
}

template<class T>
__attribute__((target("bmi,bmi2,lzcnt")))
inline constexpr int lowest_bit_pos(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);

    using u = unsigned int;
    using ul = unsigned long;
    using ull = unsigned long long;

    constexpr int DIGITS = std::numeric_limits<T>::digits;

    constexpr int DIGITS_U = std::numeric_limits<u>::digits;
    constexpr int DIGITS_UL = std::numeric_limits<ul>::digits;
    constexpr int DIGITS_ULL = std::numeric_limits<ull>::digits;

    if constexpr(DIGITS <= DIGITS_U) return __builtin_ffs(v) - 1;
    if constexpr(DIGITS <= DIGITS_UL) return __builtin_ffsl(v) - 1;
    if constexpr(DIGITS <= DIGITS_ULL) return __builtin_ffsll(v) - 1;
    else {
        static_assert(DIGITS <= DIGITS_ULL << 1);

        constexpr ull MAX_ULL = std::numeric_limits<ull>::max();

        const ull high = v >> DIGITS_ULL;
        const ull low = v & MAX_ULL;

        if(low != 0) return __builtin_ffsll(low) - 1;
        return __builtin_ffsll(high) + DIGITS_ULL - 1;
    }
}

template<class T>
inline constexpr int countr_zero(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    if(v == 0) return std::numeric_limits<T>::digits;
    return lowest_bit_pos(v);
}

template<class T> constexpr int countl_one(const T x) { return countl_zero<T>(~x); }

template<class T> constexpr int countr_one(const T x) { return countr_zero<T>(~x); }


template<class T>
inline constexpr T bit_ceil(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    if(v <= 1U) return 1;
    if constexpr(std::is_same_v<T,decltype(+v)>) return T{1} << bit_width<T>(v - 1);
    else {
        constexpr int d = std::numeric_limits<unsigned>::digits - std::numeric_limits<T>::digits;
        return T(1U << (bit_width<T>(v - 1) + d)) >> d;
    }
}

template<class T>
__attribute__((target("bmi2"))) inline constexpr T clear_higher_bits(const T v, const T p) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    assert(p < DIGITS);
    if constexpr(DIGITS <= 32) return _bzhi_u64(v, p);
    if constexpr(DIGITS <= 64) return _bzhi_u64(v, p);
    else {
        static_assert(DIGITS <= 128);

        constexpr std::uint64_t MAX64 = std::numeric_limits<std::uint64_t>::max();

        const std::uint64_t high = v >> 64;
        const std::uint64_t low = v & MAX64;

        if(p < 64) return _bzhi_u64(low, p);
        return low | (T{_bzhi_u64(high, p - 64)} << 64);
    }
}

template<class T> constexpr bool has_single_bit(const T x) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    return (x != 0) and (x & (x - 1)) == 0;
}

template<class T> constexpr T shiftl(const T, const int);
template<class T> constexpr T shiftr(const T, const int);

template<class T> constexpr T shiftl(const T x, const int n) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(n < 0) return shiftr(x, -n);
    if(n >= DIGITS) return 0;
    return x << n;
}

template<class T> constexpr T shiftr(const T x, const int n) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(n < 0) return shiftl(x, -n);
    if(n >= DIGITS) return 0;
    return x >> n;
}

template<class T> constexpr T rotl(const T x, const int n) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    constexpr unsigned U_DIGITS = static_cast<unsigned>(DIGITS);
    if constexpr(has_single_bit(U_DIGITS)) {
        const unsigned r = static_cast<unsigned>(n);
        return (x << (r % U_DIGITS)) | (x >> ((-r) % U_DIGITS));
    }
    const int r = n % DIGITS;
    if(r == 0) return x;
    if(r > 0) return (x << r) | (x >> ((DIGITS - r) % DIGITS));
    return (x >> (-r)) | (x << ((DIGITS + r) % DIGITS)); // rotr
}

template<class T> constexpr T rotr(const T x, const int n) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    constexpr unsigned U_DIGITS = static_cast<unsigned>(DIGITS);
    if constexpr(has_single_bit(U_DIGITS)) {
        const unsigned r = static_cast<unsigned>(n);
        return (x >> (r % U_DIGITS)) | (x << ((-r) % U_DIGITS)); // rotl
    }
    const int r = n % DIGITS;
    if(r == 0) return x;
    if(r > 0) return (x >> r) | (x << ((DIGITS - r) % DIGITS));
    return (x << (-r)) | (x >> ((DIGITS + r) % DIGITS));
}

#undef LIB_STATUC_ASSERT_UNSIGNED

} // namespace lib

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

    if constexpr(std::is_same_v<T,u>) return __builtin_popcount(v);
    else if constexpr(std::is_same_v<T,ul>) return __builtin_popcountl(v);
    else if constexpr(std::is_same_v<T,ull>) return __builtin_popcountll(v);
    else return __builtin_popcountll(static_cast<ull>(v));
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


template<class T>
inline constexpr T bit_ceil(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    if(v <= 1U) return 1;
    if constexpr(std::is_same_v<T,decltype(+v)>) return T{1} << bit_width<T>(v - 1);
    else {
        constexpr int d = std::numeric_limits<unsigned>::digits - std::numeric_limits<T>::digits;
        return T(1U << bit_width<T>(v - 1) + d) >> d;
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

#undef LIB_STATUC_ASSERT_UNSIGNED

} // namespace lib

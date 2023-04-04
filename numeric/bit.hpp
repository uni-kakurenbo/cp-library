#pragma once

#include <cstdint>
#include <limits>

namespace lib {

#define LIB_STATUC_ASSERT_UNSIGNED(T) static_assert(std::is_unsigned_v<T>, "only unsigned type is supported")

template<class T>
inline constexpr int countl_zero(const T v) {
    LIB_STATUC_ASSERT_UNSIGNED(T);

    using ull = unsigned long long;
    using ul = unsigned long;
    using u = unsigned;

    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(v == 0) return DIGITS;

    constexpr int DIGITS_ULL = std::numeric_limits<ull>::digits;
    constexpr int DIGITS_UL = std::numeric_limits<ul>::digits;
    constexpr int DIGITS_U = std::numeric_limits<u>::digits;

    if constexpr(DIGITS <= DIGITS_U) return __builtin_clz(v) - DIGITS_U + DIGITS;
    if constexpr(DIGITS <= DIGITS_UL) return __builtin_clzl(v) - DIGITS_UL + DIGITS;
    if constexpr(DIGITS <= DIGITS_ULL) return __builtin_clzll(v) - DIGITS_ULL + DIGITS;
    else {
        static_assert(DIGITS <= DIGITS_ULL << 1);

        constexpr ull MAX_ULL = std::numeric_limits<ull>::max();

        const ull high = v >> DIGITS_ULL;
        const ull low = v & MAX_ULL;

        if(high > 0) return __builtin_clzll(high) - (DIGITS_ULL << 1) + DIGITS;
        return  __builtin_clzll(low) - DIGITS_ULL + DIGITS;
    }
}

template<class T>
inline constexpr int bit_width(const T v) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    return std::numeric_limits<T>::digits - countl_zero(v);
}

template<class T>
inline constexpr int highest_bit_pos(const T v) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    return bit_width(v) - 1;
}


#undef LIB_STATUC_ASSERT_UNSIGNED

} // namespace lib

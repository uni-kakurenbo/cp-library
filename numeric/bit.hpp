#pragma once


#include <immintrin.h>

#include <type_traits>
#include <cstdint>
#include <limits>
#include <concepts>
#include <bit>


#include "internal/dev_env.hpp"

#include "numeric/arithmetic.hpp"


namespace lib {


template<std::unsigned_integral T>
inline constexpr int highest_bit_pos(const T v) noexcept(NO_EXCEPT) {
    return std::bit_width(v) - 1;
}

template<std::unsigned_integral T>
inline constexpr int lowest_bit_pos(const T v) noexcept(NO_EXCEPT) {
    if(v == 0) return 0;
    return std::countr_zero(v);
}


template<std::unsigned_integral T>
__attribute__((target("bmi2")))
inline constexpr T clear_higher_bits(const T v, const T p) {
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


template<std::unsigned_integral T> constexpr T shiftl(const T, const int);
template<std::unsigned_integral T> constexpr T shiftr(const T, const int);

template<std::unsigned_integral T>
constexpr T shiftl(const T x, const int n) {
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(n < 0) return shiftr(x, -n);
    if(n >= DIGITS) return 0;
    return x << n;
}

template<std::unsigned_integral T>
constexpr T shiftr(const T x, const int n) {
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(n < 0) return shiftl(x, -n);
    if(n >= DIGITS) return 0;
    return x >> n;
}


template<std::unsigned_integral T>
inline constexpr T bit(const T x, const int p) {
    return shiftr(x, p) & T{1};
}


} // namespace lib

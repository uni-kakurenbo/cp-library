#pragma once


#include <limits>
#include <concepts>
#include <cstdint>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/bit.hpp"


namespace lib {


template<std::unsigned_integral R, std::integral T>
constexpr R shrink(T x) noexcept(NO_EXCEPT) {
    constexpr int DIGITS_R = std::numeric_limits<R>::digits;
    constexpr int DIGITS_T = std::numeric_limits<R>::digits;

    REPD(digits, DIGITS_R, DIGITS_T, DIGITS_R) {
        x = (x >> digits) ^ lib::lower_bits(x, digits);
    }

    return x;
}


// Thanks to: https://gist.github.com/badboy/6267743
template<class T>
u32 hash32(T x) {
    if constexpr(std::integral<T>) {
        if constexpr(std::signed_integral<T>) return hash32(to_unsigned(x));

        constexpr int DIGITS_T = std::numeric_limits<T>::digits;

        if constexpr(DIGITS_T <= 32) {
            auto h = static_cast<u32>(x);

            h = ~h + (h << 15);
            h ^= (h >> 12);
            h += (h << 2);
            h ^= (h >> 4);
            h *= 2057;
            h ^= (h >> 16);

            return h;
        }
        else if constexpr(DIGITS_T <= 64) {
            auto h = static_cast<u64>(x);

            h = (~h) + (h << 18);
            h ^= (h >> 31);
            h *= 21;
            h ^= (h >> 11);
            h += (h << 6);
            h ^= (h >> 22);

            return static_cast<u32>(h);
        }
        else {
            return hash32(hash64(x));
        }
    }
    else {
        return hash32(std::hash<T>{}(x));
    }
}


template<class T>
u64 hash64(T x) {
    if constexpr(std::integral<T>) {
        if constexpr(std::signed_integral<T>) return hash64(to_unsigned(x));

        constexpr int DIGITS_T = std::numeric_limits<T>::digits;

        if constexpr(DIGITS_T <= 64) {
            auto h = static_cast<u64>(x);

            h = (~h) + (h << 21);
            h ^= (h >> 24);
            h *= 265;
            h ^= (h >> 14);
            h *= 21;
            h ^= (h >> 28);
            h += h << 31;

            return h;
        }
        else {
            return hash64(shrink<u64>(x));
        }
    }
    else {
        return hash64(std::hash<T>{}(x));
    }
}


} // namespace lib

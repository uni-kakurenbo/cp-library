#pragma once


#include <cstdint>
#include <limits>

#include "internal/dev_env.hpp"


namespace lib {

struct xorshift {
    using result_type = std::uint32_t;

    static constexpr result_type MIN = std::numeric_limits<result_type>::min();
    static constexpr result_type MAX = std::numeric_limits<result_type>::max();

    static constexpr result_type min() noexcept(NO_EXCEPT) { return MIN; }
    static constexpr result_type max() noexcept(NO_EXCEPT) { return MAX; }

    inline constexpr void seed(unsigned int seed) noexcept(NO_EXCEPT) { this->w = seed; }

    constexpr xorshift() = default;
    constexpr xorshift(const std::uint32_t seed) noexcept(NO_EXCEPT) : w(seed) {};

    inline constexpr result_type gen() noexcept(NO_EXCEPT) {
        result_type t;
        t = x ^ (x << 11);
        x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }
    inline constexpr result_type operator()() noexcept(NO_EXCEPT) { return this->gen(); }

  private:
    result_type x = 123456789;
    result_type y = 362436069;
    result_type z = 521288629;
    result_type w = 88675123;
};

struct xorshift64 {
    using result_type = std::uint64_t;

    static constexpr result_type MIN = std::numeric_limits<result_type>::min();
    static constexpr result_type MAX = std::numeric_limits<result_type>::max();

    static constexpr result_type min() noexcept(NO_EXCEPT) { return MIN; }
    static constexpr result_type max() noexcept(NO_EXCEPT) { return MAX; }

    inline constexpr void seed(unsigned int seed) noexcept(NO_EXCEPT) { this->x = seed; }

    constexpr xorshift64() noexcept(NO_EXCEPT) {};
    constexpr xorshift64(const std::uint64_t seed) noexcept(NO_EXCEPT) : x(seed) {};

    inline constexpr result_type gen() noexcept(NO_EXCEPT) {
        x = x ^ (x << 13), x = x ^ (x >> 7), x = x ^ (x << 17);
        return x;
    }
    inline constexpr result_type operator()() noexcept(NO_EXCEPT) { return this->gen(); }

  private:
    std::uint64_t x = 3141592653589793238UL;
};

xorshift rand;
xorshift64 rand64;


} // namespace lib

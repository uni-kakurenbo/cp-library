#pragma once


#include <cstdint>
#include <limits>
#include <bit>

#include "template/debug.hpp"


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"

#include "hash/integer_hasher.hpp"


// Thanks to: https://prng.di.unimi.it/
namespace lib {


namespace internal {


template<class Derived, class ResultType>
struct random_engine {
    using result_type = ResultType;

    static constexpr result_type MIN = std::numeric_limits<result_type>::min();
    static constexpr result_type MAX = std::numeric_limits<result_type>::max();

    static constexpr result_type min() noexcept(NO_EXCEPT) { return MIN; }
    static constexpr result_type max() noexcept(NO_EXCEPT) { return MAX; }


    template<std::unsigned_integral T = result_type>
    constexpr random_engine(const T _seed = 0) noexcept(NO_EXCEPT) {
        static_cast<Derived*>(this)->seed(_seed);
    };


    inline constexpr result_type operator()() noexcept(NO_EXCEPT) {
        return static_cast<Derived*>(this)->next();
    }
};


const i64 INTERNAL_RANDOM_GENERATOR_ID = -(1UL << 60);


};  // namespace internal


constexpr float to_float(const std::uint32_t x) noexcept(NO_EXCEPT) {
        return float(x >> 8) * 0x1.0p-24f;
}

constexpr double to_double(const std::uint64_t x) noexcept(NO_EXCEPT) {
    return double(x >> 11) * 0x1.0p-53;
}


struct mulberry32 : internal::random_engine<mulberry32, std::uint32_t> {
    using internal::random_engine<mulberry32, std::uint32_t>::random_engine;

  private:
    std::uint32_t _x;

  public:
    template<std::unsigned_integral T>
    inline constexpr void seed(const T x) noexcept(NO_EXCEPT) { this->_x = x; }

    inline constexpr std::uint32_t next() noexcept(NO_EXCEPT) {
        std::uint32_t z = (this->_x += 0x6D2B79F5U);
        z = (z ^ (z >> 15)) * (z | 1U);
        z ^= z + (z ^ (z >> 7)) * (z | 61U);
        return static_cast<std::uint32_t>(z ^ (z >> 14));
    }
};


struct splitmix64 : internal::random_engine<splitmix64, std::uint64_t> {
    using internal::random_engine<splitmix64, std::uint64_t>::random_engine;

  private:
    std::uint64_t _x;

  public:
    template<std::unsigned_integral T>
    inline constexpr void seed(const T x) noexcept(NO_EXCEPT) { this->_x = x; }

    inline constexpr std::uint64_t next() noexcept(NO_EXCEPT) {
        std::uint64_t z = (this->_x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
};



// xoroshiro64**
struct xoroshiro64ss : internal::random_engine<xoroshiro64ss, std::uint32_t> {
    using internal::random_engine<xoroshiro64ss, std::uint32_t>::random_engine;

  private:
    std::uint32_t s[2];

  public:
    template<std::unsigned_integral T>
    inline constexpr void seed(const T _seed) noexcept(NO_EXCEPT) {
        mulberry32<internal::INTERNAL_RANDOM_GENERATOR_ID> gen32(hash32(_seed));
        this->s[0] = gen32();
        this->s[1] = gen32();
    }

    inline constexpr std::uint32_t next() noexcept(NO_EXCEPT) {
        const std::uint32_t s0 = this->s[0];
        std::uint32_t s1 = this->s[1];

        const std::uint32_t res = std::rotl(s0 * 0x9E3779BBU, 5) * 5;

        s1 ^= s0;
        this->s[0] = std::rotl(s0, 26) ^ s1 ^ (s1 << 9);
        this->s[1] = std::rotl(s1, 13);

        return res;
    }
};



struct xoroshiro128ss : internal::random_engine<xoroshiro128ss, std::uint64_t> {
    using internal::random_engine<xoroshiro128ss, std::uint64_t>::random_engine;

  private:
    std::uint64_t s[2];

  public:
    template<std::unsigned_integral T>
    inline constexpr void seed(const T _seed) noexcept(NO_EXCEPT) {
        splitmix64<internal::INTERNAL_RANDOM_GENERATOR_ID> gen64(hash32(_seed));
        this->s[0] = gen64();
        this->s[1] = gen64();
    }

    inline constexpr std::uint64_t next() noexcept(NO_EXCEPT) {
        const uint64_t s0 = this->s[0];
        uint64_t s1 = this->s[1];
        const uint64_t res = std::rotl(s0 * 5, 7) * 9;

        s1 ^= s0;
        this->s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16);
        this->s[1] = std::rotl(s1, 37);

        return res;
    }
};


struct xoroshiro128pp : internal::random_engine<xoroshiro128pp, std::uint64_t> {
    using internal::random_engine<xoroshiro128pp, std::uint64_t>::random_engine;

  private:
    std::uint64_t s[2];

  public:
    template<std::unsigned_integral T>
    inline constexpr void seed(const T _seed) noexcept(NO_EXCEPT) {
        splitmix64<internal::INTERNAL_RANDOM_GENERATOR_ID> gen64(hash32(_seed));
        this->s[0] = gen64();
        this->s[1] = gen64();
    }

    inline constexpr std::uint64_t next() noexcept(NO_EXCEPT) {
        const std::uint64_t s0 = this->s[0];
        std::uint64_t s1 = this->s[1];
        const std::uint64_t res = std::rotl(s0 + s1, 17) + s0;

        s1 ^= s0;
        this->s[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21); // a, b
        this->s[1] = std::rotl(s1, 28); // c

        return res;
    }
};


// xoroshiro128+
struct xoroshiro128p : internal::random_engine<xoroshiro128p, std::uint64_t> {
    using internal::random_engine<xoroshiro128p, std::uint64_t>::random_engine;

  private:
    std::uint64_t s[2];

  public:
    template<std::unsigned_integral T>
    inline constexpr void seed(const T _seed) noexcept(NO_EXCEPT) {
        splitmix64<internal::INTERNAL_RANDOM_GENERATOR_ID> gen64(hash64(_seed));
        this->s[0] = gen64();
        this->s[1] = gen64();
    }

    inline constexpr std::uint64_t next() noexcept(NO_EXCEPT) {
        const std::uint64_t s0 = this->s[0];
        std::uint64_t s1 = this->s[1];
        const std::uint64_t res = s0 + s1;

        s1 ^= s0;
        this->s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16);
        this->s[1] = std::rotl(s1, 37);

        return res;
    }
};


struct xoroshiro64s : internal::random_engine<xoroshiro64s, std::uint32_t> {
    using internal::random_engine<xoroshiro64s, std::uint32_t>::random_engine;

  private:
    std::uint32_t s[2];

  public:
    template<std::unsigned_integral T>
    inline constexpr void seed(const T _seed) noexcept(NO_EXCEPT) {
        mulberry32<internal::INTERNAL_RANDOM_GENERATOR_ID> gen32(hash32(_seed));
        this->s[0] = gen32();
        this->s[1] = gen32();
    }

    inline constexpr std::uint32_t next() noexcept(NO_EXCEPT) {
        const std::uint32_t s0 = s[0];
        std::uint32_t s1 = s[1];
        const std::uint32_t res = s0 * 0x9E3779BB;

        s1 ^= s0;
        s[0] = std::rotl(s0, 26) ^ s1 ^ (s1 << 9);
        s[1] = std::rotl(s1, 13);

        return res;
    }
};


using random_engine_32bit = xoroshiro64ss;

using random_engine_64bit = xoroshiro128pp;

using random_engine_float = xoroshiro64s;

using random_engine_double = xoroshiro128p;



random_engine_32bit randi32;
random_engine_64bit randi64;

float randf() {
    static random_engine_float gen;
    return to_float(gen());
}

double randd() {
    static random_engine_double gen;
    return to_double(gen());
}


} // namespace lib

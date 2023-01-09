#pragma once


#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include <iterator>
#include <random>

#include "snippet/aliases.hpp"
#include "internal/types.hpp"

#include "random/xorshift.hpp"


namespace lib {


// Thanks to: https://github.com/tatyam-prime/kyopro_library/blob/master/RollingHash.cpp
template<int id = -1, std::uint64_t MOD = 0x1fffffffffffffff>
struct hasher {
  private:
    using uint64_t = std::uint64_t;
    using uint128_t = internal::uint128_t;

  public:
    using size_type = internal::size_t;
    using result_type = std::uint64_t;

    static constexpr std::uint64_t mod = MOD;
    static uint64_t base;

  private:
    static std::vector<uint64_t> power;
    std::vector<uint64_t> hashed;

    static constexpr uint64_t mask(ll a){ return (1ULL << a) - 1; }

    static constexpr uint64_t mul(uint64_t a, uint64_t b) {
        #ifdef __SIZEOF_INT128__

        uint128_t ans = uint128_t(a) * b;

        ans = (ans >> 61) + (ans & mod);
        if(ans >= mod) ans -= mod;

        #else

        uint64_t a31 = a >> 31, b31 = b >> 31;
        a &= mask(31);
        b &= mask(31);
        uint64_t x = a * b31 + b * a31;
        uint64_t ans = (a31 * b31 << 1) + (x >> 30) + ((x & mask(30)) << 31) + a * b;

        #endif

        return ans;
    }

  public:
    hasher(const size_type n = 0) {
        this->hashed.assign(n+1, 0);
        power.assign(n+1, 0);
        power[0] = 1;

        if(hasher::base > 0) return;

        xorshift64 rnd64(std::random_device{}());
        hasher::base = static_cast<std::uint64_t>(rnd64() % mod);
    }

    template<class I> hasher(const I first, const I last) : hasher(std::distance(first, last)) {
        size_type i = 0;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            power[i + 1] = mul(power[i], base);
            this->hashed[i + 1] = mul(this->hashed[i], base) + std::hash<typename std::iterator_traits<I>::value_type>{}(*itr);
            if(this->hashed[i + 1] >= mod) this->hashed[i + 1] -= mod;
        }
    }

    uint64_t get(const size_type l, const size_type r) const {
        ull res = this->hashed[r] + mod - mul(this->hashed[l], power[r - l]);
        if(res >= mod) res -= mod;
        return res;
    }
    uint64_t operator()(const size_type l, const size_type r) const {
        return this->get(l, r);
    }

    static constexpr uint64_t concat(const uint64_t h1, const uint64_t h2, const size_type len2) {
        FOR(i, static_cast<size_type>(power.size()), len2) power[i] = mul(power[i-1], base);

        ull res = mul(h1, power[len2]) + h2;
        if(res >= mod) res -= mod;

        return res;
    }

    template<class I> void concat(const I first, const I last) {
        size_type n = this->hashed.size()-1, m = std::distance(first, last);
        this->hashed.resize(n+m+1);
        power.resize(n+m+1);
        size_type i = n;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            power[i+1] = mul(power[i], base);
            this->hashed[i+1] = mul(this->hashed[i], base) + std::hash<typename std::iterator_traits<I>::value_type>{}(*itr);
            if(this->hashed[i+1] >= mod) this->hashed[i+1] -= mod;
        }
    }

    ll lcp(const hasher &b, ll l1, ll r1, ll l2, ll r2) {
        ll len = std::min(r1 - l1, r2 - l2);
        ll low = -1, high = len + 1;
        while(high - low > 1) {
            ll mid = (low + high) / 2;
            if(get(l1, l1 + mid) == b.get(l2, l2 + mid)) low = mid;
            else high = mid;
        }
        return low;
    }
};

template<int id,std::uint64_t mod>
std::uint64_t hasher<id,mod>::base = 0;

template<int id,std::uint64_t mod>
typename std::vector<std::uint64_t> hasher<id,mod>::power;


} // namespace lib

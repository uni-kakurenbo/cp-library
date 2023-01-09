#pragma once


#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include <iterator>
#include <random>

#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "internal/types.hpp"
#include "internal/dev_assert.hpp"

#include "random/xorshift.hpp"


namespace lib {


// Thanks to: https://github.com/tatyam-prime/kyopro_library/blob/master/RollingHash.cpp
template<std::uint64_t MOD = 0x1fffffffffffffff>
struct hasher {
  private:
    using uint64_t = std::uint64_t;
    using uint128_t = internal::uint128_t;

  public:
    using size_type = internal::size_t;
    using result_type = uint64_t;

    static constexpr std::uint64_t mod = MOD;
    static uint64_t base;

  private:
    size_type _n;

    std::vector<uint64_t> _hashed;

    static std::vector<uint64_t> _powers;

  protected:
    static void _allocate_powers(const size_type size) {
        if(static_cast<size_type>(hasher::_powers.size()) <= size) {
            size_type n = hasher::_powers.size();
            hasher::_powers.resize(size+1);
            if(n == 0) hasher::_powers[0] = 1;
            REP(i, std::max(0, n-1), size) hasher::_powers[i+1] = hasher::_mul(hasher::_powers[i], hasher::base);
        }
    }
    inline void _allocate(const size_type size) {
        if(static_cast<size_type>(this->_hashed.size()) <= size) this->_hashed.resize(size+1);
        hasher::_allocate_powers(size);
    }

    static constexpr uint64_t _mask(const size_type a){ return (1ULL << a) - 1; }

    static constexpr uint64_t _mul(uint64_t a, uint64_t b) {
        #ifdef __SIZEOF_INT128__

        uint128_t ans = uint128_t(a) * b;

        #else

        uint64_t a31 = a >> 31, b31 = b >> 31;
        a &= hasher::_mask(31);
        b &= hasher::_mask(31);
        uint64_t x = a * b31 + b * a31;
        uint64_t ans = (a31 * b31 << 1) + (x >> 30) + ((x & hasher::_mask(30)) << 31) + a * b;

        #endif

        ans = (ans >> 61) + (ans & hasher::mod);
        if(ans >= hasher::mod) ans -= hasher::mod;

        return ans;
    }

  public:
    struct hash {
      private:
        const uint64_t _val;
        const size_type _len;

      public:
        hash(const uint64_t _val, const size_type _len) : _val(_val), _len(_len) {}

        inline uint64_t val() const { return this->_val; }
        inline size_type len() const { return this->_len; }

        inline bool operator==(const hash& other) const { return this->_val == other._val and this->_len == other._len; }
        inline bool operator!=(const hash& other) const { return this->_val != other._val or this->_len != other._len; }

        inline hash concat(const hash& other) const { return hasher::concat(*this, other); }
        inline hash operator+(const hash& other) const { return hasher::concat(*this, other); }
    };

    hasher(const size_type n = 0) : _n(n) {
        if(hasher::base <= 0) {
            xorshift64 rand64(std::random_device{}());
            hasher::base = static_cast<std::uint64_t>(rand64() % hasher::mod);
        }
    }

    template<class I>
    hasher(const I first, const I last) : hasher(std::distance(first, last)) {
        hasher::_allocate_powers(this->_n);
        this->_hashed.assign(this->_n+1, 0);

        size_type i = 0;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            this->_hashed[i+1] = hasher::_mul(this->_hashed[i], hasher::base) + std::hash<typename std::iterator_traits<I>::value_type>{}(*itr);
            if(this->_hashed[i+1] >= hasher::mod) this->_hashed[i+1] -= hasher::mod;
        }
    }

    inline size_type size() const { return this->_n; }

    inline hash get(const size_type l, const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->_n);
        uint64_t res = this->_hashed[r] + hasher::mod - hasher::_mul(this->_hashed[l], hasher::_powers[r-l]);
        if(res >= hasher::mod) res -= hasher::mod;
        return { res, r-l };
    }
    inline hash get() const { return this->get(0, this->size()); }

    inline hash sub(const size_type p, const size_type c) const { return this->get(p, p+c); }
    inline hash sub(const size_type p) const {  return this->get(p, this->size()); }

    inline hash operator()(const size_type l, const size_type r) const { return this->get(l, r); }


    static constexpr uint64_t concat(const uint64_t h0, const uint64_t h1, const size_type len1) {
        hasher::_allocate_powers(len1);

        uint64_t res = hasher::_mul(h0, hasher::_powers[len1]) + h1;
        if(res >= hasher::mod) res -= hasher::mod;

        return res;
    }
    static constexpr hash concat(const hash& h0, const hash& h1) {
        return { hasher::concat(h0.val(), h1.val(), h1.len()), h0.len() + h1.len() };
    }


    template<class T> inline hasher& push_back(const T& v) {
        this->_n++;

        this->_allocate(this->_n);

        hasher::_powers[this->_n] = hasher::_mul(hasher::_powers[this->_n-1], hasher::base);
        this->_hashed[this->_n] = hasher::_mul(this->_hashed[this->_n-1], hasher::base) + std::hash<T>{}(v);
        if(this->_hashed[this->_n] >= hasher::mod) this->_hashed[this->_n-1] -= hasher::mod;

        return *this;
    }


    template<class I> inline hasher& concat(const I first, const I last) {
        size_type n = this->_n;
        this->_n += std::distance(first, last);

        this->_allocate(this->_n);

        size_type i = n;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            this->_hashed[i+1] = hasher::_mul(this->_hashed[i], hasher::base) + std::hash<typename std::iterator_traits<I>::value_type>{}(*itr);
            if(this->_hashed[i+1] >= hasher::mod) this->_hashed[i+1] -= hasher::mod;
        }

        return *this;
    }

    template<class T>inline hasher& concat(const T& v) { return this->concat(ALL(v)); }

    inline size_type lcp(const hasher &b, size_type l0, size_type r0, size_type l1, size_type r1) {
        size_type len = std::min(r0 - l0, r1 - l1);
        size_type low = -1, high = len + 1;

        while(high - low > 1) {
            size_type mid = (low + high) / 2;
            if(this->get(l0, l0 + mid) == b.get(l1, l1 + mid)) low = mid;
            else high = mid;
        }

        return low;
    }
};

template<std::uint64_t mod> std::uint64_t hasher<mod>::base = 0;

template<std::uint64_t mod> std::vector<std::uint64_t> hasher<mod>::_powers;


} // namespace lib

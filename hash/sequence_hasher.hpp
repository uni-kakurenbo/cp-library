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
struct sequence_hasher {
  private:
    using uint64_t = std::uint64_t;
    using uint128_t = internal::uint128_t;

  public:
    using size_type = internal::size_t;
    using hash_type = uint64_t;

    static constexpr hash_type mod = MOD;
    static hash_type base;

  private:
    size_type _n;
    std::vector<hash_type> _hashed;
    static std::vector<hash_type> _powers;

  protected:
    static inline hash_type power(const size_type p) {
        if(static_cast<size_type>(sequence_hasher::_powers.size()) <= p) {
            size_type n = sequence_hasher::_powers.size();
            sequence_hasher::_powers.resize(p+1);
            if(n == 0) sequence_hasher::_powers[0] = 1;
            REP(i, std::max(0, n-1), p) sequence_hasher::_powers[i+1] = sequence_hasher::mul(sequence_hasher::_powers[i], sequence_hasher::base);
        }
        return sequence_hasher::_powers[p];
    }

    inline hash_type& hashed(const size_type p) {
        if(static_cast<size_type>(this->_hashed.size()) <= p) this->_hashed.resize(p+1);
        return this->_hashed[p];
    }
    inline const hash_type& hashed(const size_type p) const { return this->_hashed[p]; }

    static constexpr hash_type mask(const size_type a){ return (1ULL << a) - 1; }

    static constexpr hash_type mul(hash_type a, hash_type b) {
        #ifdef __SIZEOF_INT128__

        uint128_t ans = static_cast<uint128_t>(a) * b;

        #else

        hash_type a31 = a >> 31, b31 = b >> 31;
        a &= sequence_hasher::mask(31);
        b &= sequence_hasher::mask(31);
        hash_type x = a * b31 + b * a31;
        hash_type ans = (a31 * b31 << 1) + (x >> 30) + ((x & sequence_hasher::mask(30)) << 31) + a * b;

        #endif

        ans = (ans >> 61) + (ans & sequence_hasher::mod);
        if(ans >= sequence_hasher::mod) ans -= sequence_hasher::mod;

        return ans;
    }

  public:
    struct hash {
      private:
        const hash_type _val;
        const size_type _len;

      public:
        hash(const hash_type _val, const size_type _len) : _val(_val), _len(_len) {}

        inline hash_type val() const { return this->_val; }
        inline size_type size() const { return this->_len; }

        inline bool operator==(const hash& other) const { return this->_val == other._val and this->_len == other._len; }
        inline bool operator!=(const hash& other) const { return this->_val != other._val or this->_len != other._len; }

        inline hash concat(const hash& other) const { return sequence_hasher::concat(*this, other); }
        inline hash operator+(const hash& other) const { return sequence_hasher::concat(*this, other); }
    };

    sequence_hasher(const size_type n = 0) : _n(n) {
        if(sequence_hasher::base <= 0) {
            xorshift64 rand64(std::random_device{}());
            sequence_hasher::base = static_cast<hash_type>(rand64() % sequence_hasher::mod);
        }
    }

    template<class I>
    sequence_hasher(const I first, const I last) : sequence_hasher(std::distance(first, last)) {
        this->_hashed.resize(this->_n);
        this->_hashed.assign(this->_n+1, 0);

        size_type i = 0;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            this->hashed(i+1) = sequence_hasher::mul(this->hashed(i), sequence_hasher::base) + std::hash<typename std::iterator_traits<I>::value_type>{}(*itr);
            if(this->hashed(i+1) >= sequence_hasher::mod) this->hashed(i+1) -= sequence_hasher::mod;
        }
    }

    inline size_type size() const { return this->_n; }

    inline hash get(const size_type l, const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->_n);
        hash_type res = this->hashed(r) + sequence_hasher::mod - sequence_hasher::mul(this->hashed(l), sequence_hasher::power(r-l));
        if(res >= sequence_hasher::mod) res -= sequence_hasher::mod;
        return { res, r-l };
    }
    inline hash get() const { return this->get(0, this->size()); }
    inline hash operator()(const size_type l, const size_type r) const { return this->get(l, r); }

    inline hash subseq(const size_type p, const size_type c) const { return this->get(p, p+c); }
    inline hash subseq(const size_type p) const {  return this->get(p, this->size()); }


    static constexpr hash_type concat(const hash_type h0, const hash_type h1, const size_type len1) {
        hash_type res = sequence_hasher::mul(h0, sequence_hasher::power(len1)) + h1;
        if(res >= sequence_hasher::mod) res -= sequence_hasher::mod;

        return res;
    }
    static constexpr hash concat(const hash& h0, const hash& h1) {
        return { sequence_hasher::concat(h0.val(), h1.val(), h1.size()), h0.size() + h1.size() };
    }


    template<class T> inline sequence_hasher& push_back(const T& v) {
        this->_n++;

        this->hashed(this->_n) = sequence_hasher::mul(this->hashed(this->_n-1), sequence_hasher::base) + std::hash<T>{}(v);
        if(this->hashed(this->_n) >= sequence_hasher::mod) this->hashed(this->_n-1) -= sequence_hasher::mod;

        return *this;
    }


    template<class I> inline sequence_hasher& concat(const I first, const I last) {
        size_type n = this->_n;
        this->_n += std::distance(first, last);

        size_type i = n;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            this->hashed(i+1) = sequence_hasher::mul(this->hashed(i), sequence_hasher::base) + std::hash<typename std::iterator_traits<I>::value_type>{}(*itr);
            if(this->hashed(i+1) >= sequence_hasher::mod) this->hashed(i+1) -= sequence_hasher::mod;
        }

        return *this;
    }

    template<class T>inline sequence_hasher& concat(const T& v) { return this->concat(ALL(v)); }

    inline size_type lcp(const sequence_hasher &b, size_type l0, size_type r0, size_type l1, size_type r1) {
        size_type size = std::min(r0 - l0, r1 - l1);
        size_type low = -1, high = size + 1;

        while(high - low > 1) {
            size_type mid = (low + high) / 2;
            if(this->get(l0, l0 + mid) == b.get(l1, l1 + mid)) low = mid;
            else high = mid;
        }

        return low;
    }
};

template<std::uint64_t mod> std::uint64_t sequence_hasher<mod>::base = 0;
template<std::uint64_t mod> std::vector<std::uint64_t> sequence_hasher<mod>::_powers;


} // namespace lib

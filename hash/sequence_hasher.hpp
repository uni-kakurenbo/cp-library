#pragma once


#include <cassert>
#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include <random>
#include <iterator>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/fast_prime.hpp"

#include "hash/integer_hasher.hpp"


namespace lib {


// Thanks to: https://github.com/tatyam-prime/kyopro_library/blob/master/RollingHash.cpp
template<std::uint64_t MOD = 0x1fffffffffffffff, std::uint64_t BASE = 0>
struct sequence_hasher {
  private:
    using uint64_t = std::uint64_t;
    using uint128_t = internal::uint128_t;

  public:
    using size_type = internal::size_t;
    using hash_type = uint64_t;

    static constexpr hash_type mod = MOD;
    inline static hash_type base = BASE;

  private:
    size_type _n = 0, _front = 0;
    std::vector<hash_type> _hashed;
    inline static std::vector<hash_type> _powers;

  protected:
    static inline hash_type power(const size_type p) noexcept(NO_EXCEPT) {
        if(static_cast<size_type>(sequence_hasher::_powers.size()) <= p) {
            size_type n = static_cast<size_type>(sequence_hasher::_powers.size());
            sequence_hasher::_powers.resize(p+1);
            if(n == 0) sequence_hasher::_powers[0] = 1;
            REP(i, std::max(0, n-1), p) sequence_hasher::_powers[i+1] = sequence_hasher::mul(sequence_hasher::_powers[i], sequence_hasher::base);
        }
        return sequence_hasher::_powers[p];
    }

    inline hash_type& hashed(const size_type p) noexcept(NO_EXCEPT) {
        if(static_cast<size_type>(this->_hashed.size()) <= p) this->_hashed.resize(p+1);
        return this->_hashed[p];
    }
    inline const hash_type& hashed(const size_type p) const noexcept(NO_EXCEPT) { return this->_hashed[p]; }

    static constexpr hash_type mask(const size_type a) noexcept(NO_EXCEPT) { return (1ULL << a) - 1; }

    static constexpr hash_type mul(hash_type a, hash_type b) noexcept(NO_EXCEPT) {
        #ifdef __SIZEOF_INT128__

        uint128_t res = static_cast<uint128_t>(a) * b;

        #else

        hash_type a31 = a >> 31, b31 = b >> 31;
        a &= sequence_hasher::mask(31);
        b &= sequence_hasher::mask(31);
        hash_type x = a * b31 + b * a31;
        hash_type res = (a31 * b31 << 1) + (x >> 30) + ((x & sequence_hasher::mask(30)) << 31) + a * b;

        #endif

        if constexpr(sequence_hasher::mod == 0x1fffffffffffffff) {
            res = (res >> 61) + (res & sequence_hasher::mod);
            if(res >= sequence_hasher::mod) res -= sequence_hasher::mod;
        }
        else {
            res %= sequence_hasher::mod;
        }

        return static_cast<hash_type>(res);
    }

    size_type index(size_type k) const noexcept(NO_EXCEPT) { return this->_front + k; }

  public:
    struct hash {
      private:
        const hash_type _val;
        const size_type _len;

      public:
        hash(const hash_type val, const size_type len) noexcept(NO_EXCEPT) : _val(val), _len(len) {}

        inline hash_type val() const noexcept(NO_EXCEPT) { return this->_val; }
        inline operator hash_type() const noexcept(NO_EXCEPT) { return this->_val; }

        inline size_type size() const noexcept(NO_EXCEPT) { return this->_len; }

        inline bool operator==(const hash& other) const noexcept(NO_EXCEPT) { return this->_val == other._val and this->_len == other._len; }
        inline bool operator!=(const hash& other) const noexcept(NO_EXCEPT) { return this->_val != other._val or this->_len != other._len; }

        inline hash concat(const hash& other) const noexcept(NO_EXCEPT) { return sequence_hasher::concat(*this, other); }
        inline hash operator+(const hash& other) const noexcept(NO_EXCEPT) { return sequence_hasher::concat(*this, other); }
    };

    sequence_hasher(const size_type n = 0) noexcept(NO_EXCEPT) : _n(n) {
        if(sequence_hasher::base <= 0) {
            if constexpr(BASE == 0) {
                sequence_hasher::base = static_cast<hash_type>(lib::primitive_root(sequence_hasher::mod));
            }
            else if constexpr(BASE < 0) {
                xorshift64<-(1L << 62) + 1> random(std::random_device{}());
                sequence_hasher::base = static_cast<hash_type>(random() % sequence_hasher::mod);
            }
        }
    }

    template<std::input_iterator I, std::sentinel_for<I> S>
    sequence_hasher(I first, S last) noexcept(NO_EXCEPT) : sequence_hasher(static_cast<size_type>(std::distance(first, last))) {
        this->_hashed.resize(this->_n);
        this->_hashed.assign(this->_n+1, 0);

        size_type i = 0;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            this->hashed(i+1) = sequence_hasher::mul(this->hashed(i), sequence_hasher::base) + hash64(*itr);
            if(this->hashed(i+1) >= sequence_hasher::mod) this->hashed(i+1) -= sequence_hasher::mod;
        }
    }

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n - this->_front; }

    inline hash get(size_type l, size_type r) const noexcept(NO_EXCEPT) {
        assert(0 <= l and l <= r and r <= this->size());
        l = this->index(l), r = this->index(r);
        hash_type res = this->hashed(r) + sequence_hasher::mod - sequence_hasher::mul(this->hashed(l), sequence_hasher::power(r-l));
        if(res >= sequence_hasher::mod) res -= sequence_hasher::mod;

        return { res, r - l };
    }
    inline hash get() const noexcept(NO_EXCEPT) { return this->get(0, this->size()); }
    inline hash operator()(const size_type l, const size_type r) const noexcept(NO_EXCEPT) { return this->get(l, r); }

    inline hash subseq(const size_type p, const size_type c) const noexcept(NO_EXCEPT) { return this->get(p, p+c); }
    inline hash subseq(const size_type p) const noexcept(NO_EXCEPT) {  return this->get(p, this->size()); }


    static constexpr hash_type concat(const hash_type h0, const hash_type h1, const size_type len1) noexcept(NO_EXCEPT) {
        hash_type res = sequence_hasher::mul(h0, sequence_hasher::power(len1)) + h1;
        if(res >= sequence_hasher::mod) res -= sequence_hasher::mod;

        return res;
    }
    static constexpr hash concat(const hash& h0, const hash& h1) noexcept(NO_EXCEPT) {
        return { sequence_hasher::concat(h0.val(), h1.val(), h1.size()), h0.size() + h1.size() };
    }


    template<class T> inline sequence_hasher& push_back(const T& v) noexcept(NO_EXCEPT) {
        this->_n++;

        this->hashed(this->_n) = sequence_hasher::mul(this->hashed(this->_n-1), sequence_hasher::base) + std::hash<T>{}(v);
        if(this->hashed(this->_n) >= sequence_hasher::mod) this->hashed(this->_n-1) -= sequence_hasher::mod;

        return *this;
    }

    inline sequence_hasher& pop_back() noexcept(NO_EXCEPT) {
        assert(this->_n > 0);
        this->_n--;
        return *this;
    }

    inline sequence_hasher& pop_front() noexcept(NO_EXCEPT) {
        this->_front++;
        assert(this->_front <= this->_n);
        return *this;
    }


    template<std::input_iterator I, std::sentinel_for<I> S>
    inline sequence_hasher& concat(I first, S last) noexcept(NO_EXCEPT) {
        size_type n = this->_n;
        this->_n += std::distance(first, last);

        size_type i = n;
        for(auto itr=first; itr!=last; ++i, ++itr) {
            this->hashed(i+1) = sequence_hasher::mul(this->hashed(i), sequence_hasher::base) + std::hash<typename std::iterator_traits<I>::value_type>{}(*itr);
            if(this->hashed(i+1) >= sequence_hasher::mod) this->hashed(i+1) -= sequence_hasher::mod;
        }

        return *this;
    }

    template<class T>inline sequence_hasher& concat(const T& v) noexcept(NO_EXCEPT) { return this->concat(ALL(v)); }

    inline size_type lcp(size_type l0, size_type r0, size_type l1, size_type r1) noexcept(NO_EXCEPT) {
        size_type size = std::min(r0 - l0, r1 - l1);
        size_type low = -1, high = size + 1;

        while(high - low > 1) {
            size_type mid = (low + high) / 2;
            if(this->get(l0, l0 + mid) == this->get(l1, l1 + mid)) low = mid;
            else high = mid;
        }

        return low;
    }
};


} // namespace lib

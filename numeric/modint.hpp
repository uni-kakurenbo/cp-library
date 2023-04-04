#pragma once


#include <cassert>
#include <cstdint>

#include <atcoder/modint>

#include "internal/types.hpp"


namespace lib {


template <int _mod> using static_modint = atcoder::static_modint<_mod>;
using modint998244353 = atcoder::modint998244353;
using modint1000000007 = atcoder::modint1000000007;

template <int id> using dynamic_modint = atcoder::dynamic_modint<id>;
using modint = atcoder::modint;


template<int> struct dynamic_modint_64bit;

using modint64 = dynamic_modint_64bit<-1>;


} // namespace lib


namespace lib {


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/modint/modint-montgomery64.hpp
template <int id> struct dynamic_modint_64bit : atcoder::internal::modint_base {
  private:
    using mint = dynamic_modint_64bit;

    using int64_t = std::int64_t;
    using uint64_t = std::uint64_t;
    using uint128_t = internal::uint128_t;

  protected:
    static uint64_t _mod;
    static uint64_t r;
    static uint64_t n2;

    static uint64_t get_r() {
        uint64_t res = _mod;
        for(int64_t i = 0; i < 5; ++i)
            res *= 2 - _mod * res;
        return res;
    }

    static uint64_t reduce(const uint128_t &b) {
        return (b + uint128_t(uint64_t(b) * uint64_t(-r)) * _mod) >> 64;
    }


  public:
    static uint64_t mod() { return _mod; }

    static void set_mod(const uint64_t m) {
        assert(m < (1UL << 63));
        assert((m & 1) == 1);
        _mod = m;
        n2 = -static_cast<uint128_t>(m) % m;
        r = get_r();
        assert(r * _mod == 1);
    }

    uint64_t _val;

    dynamic_modint_64bit() : _val(0) {}
    dynamic_modint_64bit(const std::int64_t b)
    : _val(this->reduce((static_cast<uint128_t>(b) + this->_mod) * this->n2)) {};

    mint &operator+=(const mint &b) {
        if(static_cast<int64_t>(_val += b._val - 2 * _mod) < 0) this->_val += 2 * this->_mod;
        return *this;
    }

    mint &operator-=(const mint &b) {
        if(static_cast<int64_t>(this->_val -= b._val) < 0)
            this->_val += 2 * this->_mod;
        return *this;
    }

    mint &operator*=(const mint &b) {
        this->_val = reduce(static_cast<uint128_t>(this->_val) * b._val);
        return *this;
    }

    mint &operator/=(const mint &b) {
        *this *= b.inv();
        return *this;
    }

    mint operator+(const mint &b) const { return mint(*this) += b; }
    mint operator-(const mint &b) const { return mint(*this) -= b; }
    mint operator*(const mint &b) const { return mint(*this) *= b; }
    mint operator/(const mint &b) const { return mint(*this) /= b; }

    bool operator==(const mint &b) const {
        return (this->_val >= this->_mod ? this->_val - this->_mod : this->_val) == (b._val >= this->_mod ? b._val - this->_mod : b._val);
    }
    bool operator!=(const mint &b) const {
        return (this->_val >= this->_mod ? this->_val - this->_mod : this->_val) != (b._val >= this->_mod ? b._val - this->_mod : b._val);
    }

    mint operator-() const { return mint{} - static_cast<mint>(*this); }

    mint pow(uint128_t n) const {
        mint res(1), mul(*this);
        while(n > 0) {
            if(n & 1)
                res *= mul;
            mul *= mul;
            n >>= 1;
        }
        return res;
    }

    mint inv() const { return this->pow(this->_mod - 2); }

    uint64_t val() const {
        uint64_t res = this->reduce(this->_val);
        return res >= this->_mod ? res - this->_mod : res;
    }
};

template<int id> typename dynamic_modint_64bit<id>::uint64_t dynamic_modint_64bit<id>::_mod;
template<int id> typename dynamic_modint_64bit<id>::uint64_t dynamic_modint_64bit<id>::r;
template<int id> typename dynamic_modint_64bit<id>::uint64_t dynamic_modint_64bit<id>::n2;


} // namespace lib

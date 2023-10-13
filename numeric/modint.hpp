#pragma once


#include <cassert>
#include <cstdint>
#include <iostream>

#include <atcoder/modint>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "snippet/aliases.hpp"


namespace lib {


namespace internal {


using atcoder::internal::is_modint;
template<class T> constexpr bool is_modint_v = is_modint<T>::value;
using atcoder::internal::is_modint_t;

template<class T> using is_static_modint = std::is_base_of<atcoder::internal::static_modint_base,T>;
template<class T> constexpr bool is_static_modint_v = is_modint<T>::value;
template<class T> using is_static_modint_t = std::enable_if_t<is_static_modint_v<T>>;

} // namespace internal


using atcoder::dynamic_modint;
using atcoder::modint;

template<u32> struct static_modint_32bit;
template<u32 Mod> using static_modint = static_modint_32bit<Mod>;
using modint998244353 = static_modint_32bit<998244353>;
using modint1000000007 = static_modint_32bit<1000000007>;

template<int> struct dynamic_modint_64bit;
using modint64 = dynamic_modint_64bit<-1>;


} // namespace lib


namespace lib {


// Thanks to: https://hackmd.io/@tatyam-prime/rkVCOcwQn
template<u32 Mod> struct static_modint_32bit : atcoder::internal::static_modint_base {
  protected:
    using mint = static_modint_32bit;
    u32 _v = 0;

  public:
    static constexpr bool prime = []() noexcept(NO_EXCEPT) -> bool {
        if(Mod == 1) return 0;
        if(Mod == 2 || Mod == 7 || Mod == 61) return 1;
        if(Mod % 2 == 0) return 0;
        u32 d = Mod - 1;
        while(d % 2 == 0) d /= 2;
        for(const u32 a : {2, 7, 61}) {
            u32 t = d;
            mint y = mint(a).pow(t);
            while(t != Mod - 1 && y != 1 && y != Mod - 1) {
                y *= y; t <<= 1;
            }
            if(y != Mod - 1 && t % 2 == 0) return 0;
        }
        return 1;
    }();

    static constexpr std::pair<i32,i32> inv_gcd(const i32 a, const i32 b) noexcept(NO_EXCEPT) {
        if(a == 0) return { b, 0 };
        i32 s = b, t = a, m0 = 0, m1 = 1;
        while(t) {
            const i32 u = s / t;
            s -= t * u; m0 -= m1 * u;
            std::swap(s, t), std::swap(m0, m1);
        }
        if(m0 < 0) m0 += b / s;
        return { s, m0 };
    }

  public:
    static constexpr mint raw(const u32 v) noexcept(NO_EXCEPT) { mint a; a._v = v; return a; }
    constexpr static_modint_32bit() noexcept(NO_EXCEPT) {}

    template <class T>
    constexpr static_modint_32bit(const T& v) noexcept(NO_EXCEPT) {
        static_assert(std::is_integral_v<T>, "T is not integral type.");
        if constexpr(std::is_signed_v<T>) {
            i64 x = i64(v % i64(Mod));
            if(x < 0) x += Mod; this->_v = u32(x);
        }
        else this->_v = u32(v % Mod);
    }

    static constexpr u32 mod() noexcept(NO_EXCEPT) { return Mod; }

    constexpr u32 val() const noexcept(NO_EXCEPT) { return this->_v; }
    explicit operator u32() const noexcept(NO_EXCEPT) { return this->_v; }

    constexpr mint& operator++() noexcept(NO_EXCEPT) { return *this += 1; }
    constexpr mint& operator--() noexcept(NO_EXCEPT) { return *this -= 1; }

    constexpr mint operator++(int) noexcept(NO_EXCEPT) { mint res = *this; ++*this; return res; }
    constexpr mint operator--(int) noexcept(NO_EXCEPT) { mint res = *this; --*this; return res; }

    constexpr mint& operator+=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(this->_v >= Mod - rhs._v) this->_v -= Mod;
        this->_v += rhs._v; return *this;
    }

    constexpr mint& operator-=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(this->_v < rhs._v) this->_v += Mod;
        this->_v -= rhs._v; return *this;
    }

    constexpr mint& operator*=(const mint& rhs) noexcept(NO_EXCEPT) { return *this = *this * rhs; }
    constexpr mint& operator/=(const mint& rhs) noexcept(NO_EXCEPT) { return *this *= rhs.inv(); }
    constexpr mint operator+() const noexcept(NO_EXCEPT) { return *this; }
    constexpr mint operator-() const noexcept(NO_EXCEPT) { return mint{} - *this; }

    constexpr mint pow(i64 n) const noexcept(NO_EXCEPT) {
        assert(0 <= n);
        if(n == 0) return 1;
        mint x = *this, r = 1;
        while(1) {
            if(n & 1) r *= x; n >>= 1;
            if(n == 0) return r;
            x *= x;
        }
    }

    constexpr mint inv() const noexcept(NO_EXCEPT) {
        if constexpr(prime) {
            assert(this->_v);
            return pow(Mod - 2);
        } else {
            auto eg = inv_gcd(this->_v, Mod);
            assert(eg.first == 1);
            return eg.second;
        }
    }

    friend constexpr mint operator+(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs -= rhs; }
    friend constexpr mint operator*(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return u64(lhs._v) * rhs._v; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs /= rhs; }
    friend constexpr bool operator==(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs._v == rhs._v; }
    friend constexpr bool operator!=(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs._v != rhs._v; }
};


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/modint/modint-montgomery64.hpp
template <int id> struct dynamic_modint_64bit : atcoder::internal::modint_base {
  private:
    using mint = dynamic_modint_64bit;

  protected:
    static u64 _mod;
    static u64 r;
    static u64 n2;

    static u64 get_r() noexcept(NO_EXCEPT) {
        u64 res = _mod;
        for(i64 i = 0; i < 5; ++i)
            res *= 2 - _mod * res;
        return res;
    }

    static u64 reduce(const u128 &b) noexcept(NO_EXCEPT) {
        return static_cast<u64>((b + static_cast<u128>(static_cast<u64>(b) * static_cast<u64>(-r)) * _mod) >> 64);
    }


  public:
    static u64 mod() noexcept(NO_EXCEPT) { return _mod; }

    static void set_mod(const u64 m) noexcept(NO_EXCEPT) {
        assert(m < (1UL << 63));
        assert((m & 1) == 1);
        _mod = m;
        n2 = static_cast<u64>(-static_cast<u128>(m) % m);
        r = get_r();
        assert(r * _mod == 1);
    }

    u64 _val;

    dynamic_modint_64bit() noexcept(NO_EXCEPT) : _val(0) {}
    dynamic_modint_64bit(const i64 b) noexcept(NO_EXCEPT)
    : _val(this->reduce((static_cast<u128>(b) + this->_mod) * this->n2)) {};

    mint &operator+=(const mint &b) noexcept(NO_EXCEPT) {
        if(static_cast<i64>(_val += b._val - 2 * _mod) < 0) this->_val += 2 * this->_mod;
        return *this;
    }

    mint &operator-=(const mint &b) noexcept(NO_EXCEPT) {
        if(static_cast<i64>(this->_val -= b._val) < 0)
            this->_val += 2 * this->_mod;
        return *this;
    }

    mint &operator*=(const mint &b) noexcept(NO_EXCEPT) {
        this->_val = reduce(static_cast<u128>(this->_val) * b._val);
        return *this;
    }

    mint &operator/=(const mint &b) noexcept(NO_EXCEPT) {
        *this *= b.inv();
        return *this;
    }

    mint operator+(const mint &b) const noexcept(NO_EXCEPT) { return mint(*this) += b; }
    mint operator-(const mint &b) const noexcept(NO_EXCEPT) { return mint(*this) -= b; }
    mint operator*(const mint &b) const noexcept(NO_EXCEPT) { return mint(*this) *= b; }
    mint operator/(const mint &b) const noexcept(NO_EXCEPT) { return mint(*this) /= b; }

    bool operator==(const mint &b) const noexcept(NO_EXCEPT) {
        return (this->_val >= this->_mod ? this->_val - this->_mod : this->_val) == (b._val >= this->_mod ? b._val - this->_mod : b._val);
    }
    bool operator!=(const mint &b) const noexcept(NO_EXCEPT) {
        return (this->_val >= this->_mod ? this->_val - this->_mod : this->_val) != (b._val >= this->_mod ? b._val - this->_mod : b._val);
    }

    mint operator-() const noexcept(NO_EXCEPT) { return mint{} - static_cast<mint>(*this); }

    mint pow(u128 n) const noexcept(NO_EXCEPT) {
        mint res(1), mul(*this);
        while(n > 0) {
            if(n & 1)
                res *= mul;
            mul *= mul;
            n >>= 1;
        }
        return res;
    }

    mint inv() const noexcept(NO_EXCEPT) { return this->pow(this->_mod - 2); }

    u64 val() const noexcept(NO_EXCEPT) {
        u64 res = this->reduce(this->_val);
        return res >= this->_mod ? res - this->_mod : res;
    }
};

template<int id> u64 dynamic_modint_64bit<id>::_mod;
template<int id> u64 dynamic_modint_64bit<id>::r;
template<int id> u64 dynamic_modint_64bit<id>::n2;


} // namespace lib

constexpr lib::modint998244353 operator""_mod998244353(unsigned long long x) { return x; }
constexpr lib::modint1000000007 operator""_mod1000000007(unsigned long long x) { return x; }

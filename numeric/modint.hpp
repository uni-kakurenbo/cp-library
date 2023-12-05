#pragma once


#include <cassert>
#include <cstdint>
#include <iostream>

#include <atcoder/modint>

#include "macro/expand_tails.hpp"

#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/internal/modint_interface.hpp"
#include "numeric/internal/primality_test.hpp"
#include "numeric/internal/primitive_root.hpp"


namespace lib {


namespace internal {


// Thanks to: https://hackmd.io/@tatyam-is_prime/rkVCOcwQn
template<std::unsigned_integral Value, std::unsigned_integral Large, Value Mod>
    requires
        (2 * std::numeric_limits<Value>::digits <= std::numeric_limits<Large>::digits) &&
        (0 < Mod)
struct static_modint_impl {
    using signed_value_type = std::make_signed_t<Value>;
    using unsigned_value_type = Value;
    using signed_large_type = std::make_signed_t<Large>;
    using unsigned_large_type = Large;

    static constexpr int digits = std::numeric_limits<unsigned_value_type>::digits;
    static constexpr unsigned_value_type max() noexcept { return std::numeric_limits<unsigned_value_type>::max(); }

  protected:
    using mint = static_modint_impl;
    unsigned_value_type _v = 0;

  public:
    static constexpr bool is_prime = lib::internal::is_prime<static_modint_32bit, static_modint_64bit>(Mod);
    //static constexpr bool primitive_root = lib::internal::primitive_root<static_modint_32bit, static_modint_64bit>(Mod);

    static constexpr spair<signed_value_type> inv_gcd(const signed_value_type a, const signed_value_type b) noexcept(NO_EXCEPT) {
        if(a == 0) return { b, 0 };
        signed_value_type s = b, t = a, m0 = 0, m1 = 1;
        while(t) {
            const signed_value_type u = s / t;
            s -= t * u; m0 -= m1 * u;
            std::swap(s, t), std::swap(m0, m1);
        }
        if(m0 < 0) m0 += b / s;
        return { s, m0 };
    }

  public:
    static constexpr mint raw(const unsigned_value_type v) noexcept(NO_EXCEPT) {
        mint res;
        res._v = v;
        return res;
    }

    constexpr static_modint_impl() noexcept(NO_EXCEPT) {}

    template<std::integral T>
    constexpr static_modint_impl(const T& v) noexcept(NO_EXCEPT) {
        if constexpr(std::is_signed_v<T>) {
            signed_large_type x = signed_large_type(v % signed_large_type(Mod));
            if(x < 0) x += Mod; this->_v = unsigned_value_type(x);
        }
        else this->_v = unsigned_value_type(v % Mod);
    }

    static constexpr unsigned_value_type mod() noexcept(NO_EXCEPT) { return Mod; }

    constexpr unsigned_value_type val() const noexcept(NO_EXCEPT) { return this->_v; }
    explicit operator unsigned_value_type() const noexcept(NO_EXCEPT) { return this->_v; }

    constexpr mint& operator+=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(this->_v >= Mod - rhs._v) this->_v -= Mod;
        this->_v += rhs._v; return *this;
    }

    constexpr mint& operator-=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(this->_v < rhs._v) this->_v += Mod;
        this->_v -= rhs._v; return *this;
    }


    constexpr mint& operator++() noexcept(NO_EXCEPT) { return *this += 1; }
    constexpr mint& operator--() noexcept(NO_EXCEPT) { return *this -= 1; }

    constexpr mint operator++(int) noexcept(NO_EXCEPT) { mint res = *this; ++*this; return res; }
    constexpr mint operator--(int) noexcept(NO_EXCEPT) { mint res = *this; --*this; return res; }


    constexpr mint& operator*=(const mint& rhs) noexcept(NO_EXCEPT) { return *this = *this * rhs; }
    constexpr mint& operator/=(const mint& rhs) noexcept(NO_EXCEPT) { return *this *= rhs.inv(); }
    constexpr mint operator+() const noexcept(NO_EXCEPT) { return *this; }
    constexpr mint operator-() const noexcept(NO_EXCEPT) { return mint{} - *this; }


    constexpr mint pow(i64 n) const noexcept(NO_EXCEPT) {
        assert(0 <= n);

        mint res(1), mul(*this);

        while(n > 0) {
            if(n & 1) res *= mul;
            mul *= mul;
            n >>= 1;
        }

        return res;
    }

    constexpr mint inv() const noexcept(NO_EXCEPT) {
        if constexpr(mint::is_prime) {
            assert(this->_v);
            return this->pow(Mod - 2);
        } else {
            auto eg = this->inv_gcd(this->_v, Mod);
            assert(eg.first == 1);
            return eg.second;
        }
    }

    friend constexpr mint operator+(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs -= rhs; }
    friend constexpr mint operator*(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return unsigned_large_type(lhs._v) * rhs._v; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs /= rhs; }
    friend constexpr bool operator==(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs._v == rhs._v; }
    friend constexpr bool operator!=(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs._v != rhs._v; }
};


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/modint/modint-montgomery64.hpp
template<std::unsigned_integral Value, std::unsigned_integral Large, i64 Id>
    requires (2 * std::numeric_limits<Value>::digits <= std::numeric_limits<Large>::digits)
struct dynamic_modint_impl {
    using signed_value_type = std::make_signed_t<Value>;
    using unsigned_value_type = Value;
    using signed_large_type = std::make_signed_t<Large>;
    using unsigned_large_type = Large;

    static constexpr int digits = std::numeric_limits<unsigned_value_type>::digits - 2;
    static constexpr unsigned_value_type max() noexcept { return (unsigned_value_type{ 1 } << mint::digits) - 1; }

  private:
    using mint = dynamic_modint_impl;

    unsigned_value_type _val;

    static unsigned_value_type _mod;
    static unsigned_value_type _r;
    static unsigned_value_type _n2;

    constexpr static unsigned_value_type get_r() noexcept(NO_EXCEPT) {
        unsigned_value_type res = mint::_mod;
        while(mint::_mod * res != 1) res *= unsigned_value_type{ 2 } - mint::_mod * res;
        return res;
    }

    constexpr static unsigned_value_type reduce(const unsigned_large_type &v) noexcept(NO_EXCEPT) {
        return
            static_cast<unsigned_value_type>(
                (
                    v +
                    static_cast<unsigned_large_type>(
                        static_cast<unsigned_value_type>(v) * static_cast<unsigned_value_type>(-_r)
                    ) * _mod
                ) >> std::numeric_limits<unsigned_value_type>::digits
            );
    }

    inline unsigned_value_type normalized() const noexcept(NO_EXCEPT) {
        if(this->_val < mint::_mod) return this->_val;
        return this->_val - mint::_mod;
    }

  public:
    static constexpr unsigned_value_type mod() noexcept(NO_EXCEPT) { return _mod; }

    static constexpr mint raw(const unsigned_large_type v) noexcept(NO_EXCEPT)
    {
        mint res;
        res._val = mint::reduce(v * mint::_n2);
        return res;
    };

    static constexpr void set_mod(const unsigned_value_type m) noexcept(NO_EXCEPT) {
        assert(m < mint::max());
        assert((m & 1) == 1);

        if(mint::mod() == m) return;

        mint::_mod = m;
        mint::_n2 = static_cast<unsigned_value_type>(-static_cast<unsigned_large_type>(m) % m);
        mint::_r = mint::get_r();

        assert(mint::_r * mint::_mod == 1);

        return;
    }

    constexpr unsigned_value_type val() const noexcept(NO_EXCEPT) {
        unsigned_value_type res = mint::reduce(this->_val);
        return res >= mint::_mod ? res - mint::_mod : res;
    }

    constexpr dynamic_modint_impl() noexcept(NO_EXCEPT) : _val(0) {}

    template<std::integral T>
    constexpr dynamic_modint_impl(T v) noexcept(NO_EXCEPT) {
        using common_type = std::common_type_t<T, unsigned_value_type>;

        const common_type m = static_cast<common_type>(mint::_mod);

        if constexpr(std::is_signed_v<T>) {
            if(
                (v > 0 && static_cast<common_type>(v) > m) ||
                (v < 0 && static_cast<common_type>(-v) > m)
            ) v %= m;
            if(v < 0) v += m;
        }
        else {
            if(static_cast<common_type>(v) >= m) v %= m;
        }

        this->_val = mint::reduce(static_cast<unsigned_large_type>(v) * mint::_n2);
    }


    constexpr mint& operator+=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(static_cast<signed_value_type>(this->_val += rhs._val - 2 * _mod) < 0) this->_val += 2 * mint::_mod;
        return *this;
    }

    constexpr mint& operator-=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(static_cast<signed_value_type>(this->_val -= rhs._val) < 0) this->_val += 2 * mint::_mod;
        return *this;
    }

    constexpr mint& operator*=(const mint& rhs) noexcept(NO_EXCEPT) {
        this->_val = mint::reduce(static_cast<unsigned_large_type>(this->_val) * rhs._val);
        return *this;
    }

    constexpr mint& operator/=(const mint& rhs) noexcept(NO_EXCEPT) {
        *this *= rhs.inv(); return *this;
    }


    constexpr mint& operator++() noexcept(NO_EXCEPT) { return *this += 1; }
    constexpr mint& operator--() noexcept(NO_EXCEPT) { return *this -= 1; }

    constexpr mint operator++(int) noexcept(NO_EXCEPT) { mint res = *this; ++*this; return res; }
    constexpr mint operator--(int) noexcept(NO_EXCEPT) { mint res = *this; --*this; return res; }


    constexpr mint operator+() const noexcept(NO_EXCEPT) { return *this; }
    constexpr mint operator-() const noexcept(NO_EXCEPT) { return mint{} - *this; }


    constexpr mint pow(i64 n) const noexcept(NO_EXCEPT) {
        assert(0 <= n);

        mint res{ 1 }, mul = *this;

        while(n > 0) {
            if(n & 1) res *= mul;
            mul *= mul;
            n >>= 1;
        }

        return res;
    }

    constexpr mint inv() const {
        signed_value_type x = this->val(), y = mint::mod(), u = 1, v = 0;
        while(y > 0) {
            signed_value_type t = x / y;
            std::swap(x -= t * y, y);
            std::swap(u -= t * v, v);
        }
        return mint{ u };
    }


    friend constexpr bool operator==(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs.normalized() == rhs.normalized(); }
    friend constexpr bool operator!=(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs.normalized() != rhs.normalized(); }

    friend constexpr mint operator+(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) noexcept(NO_EXCEPT) { return lhs /= rhs; }
};


EXPAND_TAILS(
    (
        template<std::unsigned_integral Value, std::unsigned_integral Large, i64 Id>
            requires (2 * std::numeric_limits<Value>::digits <= std::numeric_limits<Large>::digits)
        Value dynamic_modint_impl<Value, Large, Id>::
    ),
    (_mod), (_r), (_n2)
)


} // namespace internal


} // namespace lib

constexpr lib::modint998244353 operator""_mod998244353(unsigned long long x) { return x; }
constexpr lib::modint1000000007 operator""_mod1000000007(unsigned long long x) { return x; }

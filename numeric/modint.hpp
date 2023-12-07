#pragma once


#include <cassert>
#include <cstdint>
#include <iostream>


#include "macro/expand_tails.hpp"

#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/internal/modint_interface.hpp"
#include "numeric/internal/primality_test.hpp"
#include "numeric/internal/primitive_root.hpp"
#include "numeric/internal/barrett.hpp"
#include "numeric/internal/montgomery.hpp"

#include "template/debug.hpp"


namespace lib {


namespace internal {


template<std::unsigned_integral Value, std::unsigned_integral Large, Value Mod>
    requires has_double_digits_of<Large, Value> && (Mod > 0)
struct static_modint_impl : modint_interface<static_modint_impl<Value, Large, Mod>, Value> {
    using signed_value_type = std::make_signed_t<Value>;
    using unsigned_value_type = Value;
    using signed_large_type = std::make_signed_t<Large>;
    using unsigned_large_type = Large;

    static constexpr int digits = std::numeric_limits<unsigned_value_type>::digits;
    static inline constexpr unsigned_value_type max() noexcept { return std::numeric_limits<unsigned_value_type>::max(); }


  private:
    using mint = static_modint_impl;
    using base = modint_interface<static_modint_impl<Value, Large, Mod>, Value>;

    unsigned_value_type _val = 0;


  public:
    static constexpr bool is_prime = lib::internal::is_prime<mint>(Mod);
    static constexpr unsigned_value_type primitive_root = lib::internal::primitive_root<mint>(Mod);

    static constexpr mint zero = 0;
    static constexpr mint one = 1;

    static inline constexpr unsigned_value_type mod() noexcept(NO_EXCEPT) { return Mod; }

    static inline constexpr mint raw(const unsigned_value_type v) noexcept(NO_EXCEPT) {
        mint res;
        res._val = v;
        return res;
    }


    constexpr static_modint_impl() noexcept = default;

    template<std::integral T>
    constexpr static_modint_impl(T v) noexcept(NO_EXCEPT) {
        using common_type = std::common_type_t<T, unsigned_value_type>;
        const common_type m = static_cast<common_type>(Mod);

        v %= m;

        if constexpr(std::is_signed_v<T>) {
            if(v < 0) v += m;
        }

        this->_val = v;
    }


    inline constexpr unsigned_value_type val() const noexcept(NO_EXCEPT) { return this->_val; }


    inline constexpr mint& operator+=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(this->_val >= Mod - rhs._val) this->_val -= Mod;
        this->_val += rhs._val;
        return *this;
    }

    inline constexpr mint& operator-=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(this->_val < rhs._val) this->_val += Mod;
        this->_val -= rhs._val;
        return *this;
    }

    inline constexpr mint& operator*=(const mint& rhs) noexcept(NO_EXCEPT) {
        this->_val =
            static_cast<unsigned_value_type>(
                static_cast<unsigned_large_type>(this->_val) * rhs._val % Mod
            );
        return *this;
    }

    constexpr mint inv() const noexcept(NO_EXCEPT) {
        if constexpr(mint::is_prime) {
            assert(this->_val);
            return this->pow(Mod - 2);
        } else
        {
            return this->base::inv();
        }
    }

    friend inline constexpr bool operator==(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) {
        return lhs._val == rhs._val;
    }
};


template<std::unsigned_integral Value, std::unsigned_integral Large, i64 Id>
    requires has_double_digits_of<Large, Value>
struct barrett_modint_impl : modint_interface<barrett_modint_impl<Value, Large, Id>, Value> {
    using signed_value_type = std::make_signed_t<Value>;
    using unsigned_value_type = Value;
    using signed_large_type = std::make_signed_t<Large>;
    using unsigned_large_type = Large;

  private:
    using mint = barrett_modint_impl;
    using barrett = barrett_context<unsigned_value_type, unsigned_large_type>;

    unsigned_value_type _val = 0;

    static inline barrett _barrett;

  public:
    static constexpr int digits = barrett::digits;
    static inline constexpr unsigned_value_type max() noexcept { return barrett::max(); }

    static inline mint zero;
    static inline mint one = 1;

    static inline constexpr unsigned_value_type mod() noexcept(NO_EXCEPT) { return mint::_barrett.mod(); }

    static inline constexpr void set_mod(const unsigned_value_type m) noexcept(NO_EXCEPT) {
        if(mint::mod() == m) return;
        mint::_barrett = mint::barrett(m);
    }

    static inline constexpr mint raw(const unsigned_value_type v) noexcept(NO_EXCEPT)
    {
        mint res;
        res._val = v;
        return res;
    };

    constexpr barrett_modint_impl() noexcept = default;

    template<std::integral T>
    constexpr barrett_modint_impl(const T v) noexcept(NO_EXCEPT) : _val(mint::_barrett.convert(v)) {}


    inline constexpr unsigned_value_type val() const noexcept(NO_EXCEPT) { return this->_val; }


    inline constexpr mint& operator+=(const mint& rhs) noexcept(NO_EXCEPT) {
        if((this->_val += rhs._val) >= mint::mod()) this->_val -= mint::mod();
        return *this;
    }

    inline constexpr mint& operator-=(const mint& rhs) noexcept(NO_EXCEPT) {
        if((this->_val += mint::mod() - rhs._val) >= mint::mod()) this->_val -= mint::mod();
        return *this;
    }

    inline constexpr mint& operator*=(const mint& rhs) noexcept(NO_EXCEPT) {
        this->_val = mint::_barrett.multiply(this->_val, rhs._val);
        return *this;
    }

    friend inline constexpr bool operator==(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) {
        return lhs._val == rhs._val;
    }
};


template<std::unsigned_integral Value, std::unsigned_integral Large, i64 Id>
    requires has_double_digits_of<Large, Value>
struct montgomery_modint_impl : modint_interface<montgomery_modint_impl<Value, Large, Id>, Value> {
    using signed_value_type = std::make_signed_t<Value>;
    using unsigned_value_type = Value;
    using signed_large_type = std::make_signed_t<Large>;
    using unsigned_large_type = Large;


  private:
    using mint = montgomery_modint_impl;
    using montgomery = montgomery_context<unsigned_value_type, unsigned_large_type>;

    unsigned_value_type _val = 0;

    static inline montgomery _montgomery;


  public:
    static constexpr int digits = mint::montgomery::digits;
    static inline constexpr unsigned_value_type max() noexcept { return mint::montgomery::max(); }


    static inline mint zero;
    static inline mint one;


    static inline constexpr unsigned_value_type mod() noexcept(NO_EXCEPT) { return mint::_montgomery.mod(); }

    static inline constexpr void set_mod(const unsigned_value_type m) noexcept(NO_EXCEPT) {
        mint::_montgomery = mint::montgomery(m);
        mint::one._val = mint::_montgomery.reduce(mint::_montgomery.r2());
    }

    static inline constexpr mint raw(const unsigned_large_type v) noexcept(NO_EXCEPT)
    {
        mint res;
        res._val = mint::_montgomery.convert_raw(v);
        return res;
    };


    constexpr montgomery_modint_impl() noexcept = default;

    template<std::integral T>
    constexpr montgomery_modint_impl(const T v) noexcept(NO_EXCEPT) : _val(mint::_montgomery.convert(v)) {}


    inline constexpr unsigned_value_type val() const noexcept(NO_EXCEPT) {
        return mint::_montgomery.normalize(mint::_montgomery.reduce(this->_val));
    }


    inline constexpr mint& operator+=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(static_cast<signed_value_type>(this->_val += rhs._val - (mint::mod() << 1)) < 0) this->_val += mint::mod() << 1;
        return *this;
    }

    inline constexpr mint& operator-=(const mint& rhs) noexcept(NO_EXCEPT) {
        if(static_cast<signed_value_type>(this->_val -= rhs._val) < 0) this->_val += mint::mod() << 1;
        return *this;
    }

    inline constexpr mint& operator*=(const mint& rhs) noexcept(NO_EXCEPT) {
        this->_val = mint::_montgomery.multiply(this->_val, rhs._val);
        return *this;
    }

    friend inline constexpr bool operator==(const mint& lhs, const mint& rhs) noexcept(NO_EXCEPT) {
        return mint::_montgomery.equal(lhs._val, rhs._val);
    }
};


} // namespace internal


} // namespace lib


constexpr lib::modint998244353 operator""_mod998244353(unsigned long long x) { return x; }
constexpr lib::modint1000000007 operator""_mod1000000007(unsigned long long x) { return x; }

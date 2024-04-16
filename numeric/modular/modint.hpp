#pragma once


#include <cassert>
#include <cstdint>
#include <iostream>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/modular/modint_interface.hpp"
// #include "numeric/internal/primality_test.hpp"
// #include "numeric/internal/primitive_root.hpp"

#include "template/debug.hpp"


namespace uni {


namespace internal {


template<class, bool>
struct modint_base {};


template<class Mint>
struct modint_base<Mint, false> {
    static constexpr Mint zero = Mint::_raw(0);
    static constexpr Mint one = Mint::_one();
};


template<class Mint>
struct modint_base<Mint, true> {
    static constexpr Mint zero = Mint::_raw(0);
    static inline Mint one = Mint::_one();
};


} // internal



template<internal::modular_context Context>
struct modint : internal::modint_base<modint<Context>, Context::dynamic> {
    using value_type = typename Context::value_type;
    using context = Context;

  private:
    using base = internal::modint_base<modint, context::dynamic>;

    friend base;

    value_type _val = 0;


    static constexpr modint _raw(const value_type v) noexcept(NO_EXCEPT) {
        modint res;
        res._val = v;
        return res;
    }


    static constexpr modint _one() noexcept(NO_EXCEPT)
        requires internal::has_static_one<typename modint::context::reductor>
    {
        return modint::_raw(modint::context::reduction.one);
    }

    static constexpr modint _one() noexcept(NO_EXCEPT)
        requires (!internal::has_static_one<typename modint::context::reductor>)
    {
        return modint::_raw(1);
    }

  public:
    static constexpr int digits = modint::context::reductor::digits;
    static constexpr value_type max() noexcept { return modint::context::reductor::max(); }


    static constexpr void set_mod(const value_type mod) noexcept(NO_EXCEPT)
        requires requires(value_type mod) { modint::context::set_mod(mod); }
    {
        modint::context::set_mod(mod);
        modint::one = modint::_one();
    }


    static constexpr value_type mod() noexcept(NO_EXCEPT) { return modint::context::reduction.mod(); }

    static constexpr modint raw(const value_type v) noexcept(NO_EXCEPT)
    {
        modint res;
        res._val = modint::context::reduction.convert_raw(v);
        return res;
    };


    constexpr modint() noexcept = default;

    template<std::integral T>
    constexpr modint(const T v) noexcept(NO_EXCEPT) : _val(modint::context::reduction.convert(v)) {}


    inline constexpr value_type val() const noexcept(NO_EXCEPT) {
        return modint::context::reduction.revert(this->_val);
    }

    inline constexpr explicit operator value_type() const noexcept(NO_EXCEPT) { return this->_val; }


    inline constexpr modint& operator+=(const modint& rhs) noexcept(NO_EXCEPT) {
        this->_val = modint::context::reduction.add(this->_val, rhs._val);
        return *this;
    }

    inline constexpr modint& operator-=(const modint& rhs) noexcept(NO_EXCEPT) {
        this->_val = modint::context::reduction.subtract(this->_val, rhs._val);
        return *this;
    }


    inline constexpr modint& operator*=(const modint& rhs) noexcept(NO_EXCEPT) {
        this->_val = modint::context::reduction.multiply(this->_val, rhs._val);
        return *this;
    }

    inline constexpr auto& operator/=(const modint& rhs) noexcept(NO_EXCEPT) { return *this *= rhs.inv(); }


    constexpr modint pow(const i64 n) const noexcept(NO_EXCEPT) {
        return modint::_raw(modint::context::reduction.pow(this->_val, n));
    }


    constexpr modint inv() const noexcept(NO_EXCEPT) {
        using signed_value_type = std::make_signed_t<value_type>;

        signed_value_type x = this->_val, y = modint::mod(), u = 1, v = 0;

        while(y > 0) {
            signed_value_type t = x / y;
            std::swap(x -= t * y, y);
            std::swap(u -= t * v, v);
        }

        assert(x == 1);

        if(u < 0) u += v / x;
        return modint::raw(u);
    }


    friend inline constexpr auto operator<=>(const modint& lhs, const modint& rhs) noexcept(NO_EXCEPT) {
        return modint::context::reduction.compare(lhs._val, rhs._val);
    }

    friend inline constexpr bool operator==(const modint& lhs, const modint& rhs) noexcept(NO_EXCEPT) {
        return lhs <=> rhs == 0;
    }


    inline constexpr modint& operator++() noexcept(NO_EXCEPT) { return *this += modint::one; }
    inline constexpr modint& operator--() noexcept(NO_EXCEPT) { return *this -= modint::one; }

    inline constexpr modint operator++(int) noexcept(NO_EXCEPT) { const modint res = *this; return ++*this, res; }
    inline constexpr modint operator--(int) noexcept(NO_EXCEPT) { const modint res = *this; return --*this, res; }

    inline constexpr auto operator+() const noexcept(NO_EXCEPT) { return *this; }
    inline constexpr auto operator-() const noexcept(NO_EXCEPT) { return modint::zero - *this; }

    friend inline constexpr modint operator+(modint lhs, const modint& rhs) noexcept(NO_EXCEPT) { return lhs += rhs; }
    friend inline constexpr modint operator-(modint lhs, const modint& rhs) noexcept(NO_EXCEPT) { return lhs -= rhs; }
    friend inline constexpr modint operator*(modint lhs, const modint& rhs) noexcept(NO_EXCEPT) { return lhs *= rhs; }
    friend inline constexpr modint operator/(modint lhs, const modint& rhs) noexcept(NO_EXCEPT) { return lhs /= rhs; }
};


} // namespace uni

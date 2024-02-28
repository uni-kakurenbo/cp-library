#pragma once


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"

#include "algebraic/base.hpp"
#include "numeric/fast_prime.hpp"
#include "numeric/modular/modint.hpp"


namespace lib {

namespace algebraic {


template<lib::internal::modint_family ValueType, typename ValueType::value_type BASE>
struct rolling_hash_impl {
    using value_type = ValueType;
    inline static value_type base;

  private:
    value_type _value = 0, _power = 1;

  public:
    rolling_hash_impl(const value_type& value, const value_type& power) noexcept(NO_EXCEPT)
      : _value(value), _power(power)
    {
        if(rolling_hash_impl::base == 0) {
            if constexpr(BASE == 0) {
                rolling_hash_impl::base = lib::primitive_root(value_type::mod());
            }
            else if constexpr(BASE < 0) {
                rolling_hash_impl::base = static_cast<value_type>(lib::primitive_root(rolling_hash_impl::mod));
            }
            else {
                rolling_hash_impl::base = BASE;
            }
        }
    }

    rolling_hash_impl(const value_type& value) noexcept(NO_EXCEPT) : rolling_hash_impl(value, 0) {
        this->_power = rolling_hash_impl::base;
    };

    rolling_hash_impl() noexcept = default;


    inline value_type power() const noexcept(NO_EXCEPT) { return this->_power; }
    inline value_type val() const noexcept(NO_EXCEPT) { return this->_value; }


    friend inline bool operator==(const rolling_hash_impl& lhs, const rolling_hash_impl& rhs) noexcept(NO_EXCEPT) {
        return lhs._value == rhs._value;
    }

    friend bool operator!=(const rolling_hash_impl& lhs, const rolling_hash_impl& rhs) noexcept(NO_EXCEPT) {
        return lhs._value != rhs._value;
    }
};


template<
    bool REVERSE = false,
    lib::internal::modint_family T = lib::static_modint_64bit<(1UL << 61) - 1>,
    typename T::value_type BASE = 0
>
struct rolling_hash : base<rolling_hash_impl<T, BASE>>, associative {
    using base<rolling_hash_impl<T, BASE>>::base;

    template<class U>
    rolling_hash(const U& v) : base<rolling_hash_impl<T, BASE>>({ std::hash<U>{}(v) }) {}

    friend inline rolling_hash operator+(const rolling_hash& lhs, const rolling_hash& rhs) noexcept(NO_EXCEPT) {
        const auto power = lhs->power() * rhs->power();
        if constexpr(REVERSE) return rolling_hash({ lhs->val() * rhs->power() + rhs->val(), power });
        return rolling_hash({ lhs->val() + rhs->val() * lhs->power(), power });
    }

    friend inline rolling_hash operator-(const rolling_hash& lhs, const rolling_hash& rhs) noexcept(NO_EXCEPT) {
        const auto rhs_power_inv = rhs->power.inv();
        const auto power = lhs->power() * rhs_power_inv;
        if constexpr(REVERSE) return rolling_hash({ (lhs->val() - rhs->val()) * rhs_power_inv, power });
        return rolling_hash({ lhs->val() - rhs->val() * lhs->power(), power });
    }
};


} // namespace algebraic

} // namespace lib

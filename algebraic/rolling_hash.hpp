#pragma once


#include <compare>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"

#include "algebraic/base.hpp"

#include "numeric/fast_prime.hpp"
#include "numeric/modular/modint.hpp"

#include "random/engine.hpp"


namespace uni {

namespace algebraic {


template<uni::internal::modint_family ValueType, typename ValueType::value_type BASE>
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
                rolling_hash_impl::base = uni::primitive_root<true>(value_type::mod());
            }
            else if constexpr(BASE < 0) {
                random_engine_64bit random(std::random_device{}());
                rolling_hash_impl::base = static_cast<value_type>(random() % value_type::mod());
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


    inline auto power() const noexcept(NO_EXCEPT) { return this->_power; }
    inline auto val() const noexcept(NO_EXCEPT) { return this->_value; }


    friend inline bool operator==(const rolling_hash_impl& lhs, const rolling_hash_impl& rhs) noexcept(NO_EXCEPT) {
        return lhs._power == rhs._power && lhs._value == rhs._value;
    }

    friend inline auto operator<=>(const rolling_hash_impl& lhs, const rolling_hash_impl& rhs) noexcept(NO_EXCEPT) {
        const auto comp_power = lhs._power <=> rhs._power;
        if(comp_power != 0) return comp_power;
        return lhs._value <=> rhs._value;
    }
};


template<
    bool REVERSE = false,
    uni::internal::modint_family T = uni::static_modint_64bit<(1UL << 61) - 1>,
    typename T::value_type BASE = 0
>
struct rolling_hash : base<rolling_hash_impl<T, BASE>>, scalar_multipliable<rolling_hash<REVERSE, T, BASE>>::automatic, associative {
    using base<rolling_hash_impl<T, BASE>>::base;


    rolling_hash(const T& v) : base<rolling_hash_impl<T, BASE>>(v) {}

    template<class U>
        requires (
            (!std::same_as<std::remove_cvref_t<U>, T>) &&
            (!std::constructible_from<rolling_hash_impl<T, BASE>, U>)
        )
    rolling_hash(U&& v) : base<rolling_hash_impl<T, BASE>>(hash64(std::forward<U>(v))) {}


    friend inline auto operator+(const rolling_hash& lhs, const rolling_hash& rhs) noexcept(NO_EXCEPT) {
        const auto power = lhs->power() * rhs->power();
        if constexpr(REVERSE) return rolling_hash({ lhs->val() * rhs->power() + rhs->val(), power });
        return rolling_hash({ lhs->val() + rhs->val() * lhs->power(), power });
    }

    inline auto operator-() noexcept(NO_EXCEPT) {
        const auto power_inv = this->val().power().inv();
        return rolling_hash({ -this->val().val() * power_inv, power_inv });
    }
};


} // namespace algebraic

} // namespace uni

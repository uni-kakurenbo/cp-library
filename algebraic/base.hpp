#pragma once


#include <utility>
#include <concepts>


#include "internal/dev_env.hpp"
#include "numeric/arithmetic.hpp"


namespace lib {

namespace algebraic {


template<class Derived>
struct scalar_multipliable {
    struct identity {
        template<std::integral Scalar>
        friend inline Derived operator*(const Scalar, const Derived& val) noexcept(NO_EXCEPT) {
            return val;
        }
    };


    struct automatic {
        template<std::integral Scalar>
        friend inline Derived operator*(const Scalar k, const Derived& val) noexcept(NO_EXCEPT) {
            return lib::pow<Derived, Scalar, std::plus<Derived>>(val, k, {}, {});
        }
    };
};



template<class T>
struct base {
    using value_type = T;

  protected:
    value_type _value;

  public:
    template<class... Args>
        requires std::constructible_from<value_type, Args...>
    base(Args&&... args) noexcept(NO_EXCEPT) : _value(std::forward<Args>(args)...) {}

    inline explicit operator value_type() const noexcept(NO_EXCEPT) { return this->_value; }
    inline value_type val() const noexcept(NO_EXCEPT) { return this->_value; };

    inline const value_type* operator->() const noexcept(NO_EXCEPT) { return &this->_value; };
    inline value_type* operator->() noexcept(NO_EXCEPT) { return &this->_value; };

    friend inline bool operator==(const base& lhs, const base& rhs) noexcept(NO_EXCEPT) { return lhs._value == rhs._value; };
    friend bool operator!=(const base& lhs, const base& rhs) noexcept(NO_EXCEPT) { return lhs._value != rhs._value; };
};

struct associative {};

struct commutative {};


} // namespace algebraic

} // namespace lib

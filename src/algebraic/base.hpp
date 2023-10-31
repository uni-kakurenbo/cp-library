#pragma once

#include "internal/dev_env.hpp"


namespace lib {

namespace algebraic {


template<class T>
struct base {
    using value_type = T;

  protected:
    value_type _value;

  public:
    base(const value_type& value = {}) noexcept(NO_EXCEPT) : _value(value) {};

    inline explicit operator value_type() const noexcept(NO_EXCEPT) { return this->_value; }
    inline value_type val() const noexcept(NO_EXCEPT) { return this->_value; };

    inline const value_type* operator->() const noexcept(NO_EXCEPT) { return &this->_value; };
    inline value_type* operator->() noexcept(NO_EXCEPT) { return &this->_value; };

    friend inline bool operator==(const base& lhs, const base& rhs) noexcept(NO_EXCEPT) { return lhs._value == rhs._value; };
    friend bool operator!=(const base& lhs, const base& rhs) noexcept(NO_EXCEPT) { return lhs._value != rhs._value; };
};


struct associative {};

struct commutative {};

struct invertible {};


} // namespace algebraic

} // namespace lib

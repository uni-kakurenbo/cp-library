#pragma once

#include "internal/dev_env.hpp"


namespace lib {

namespace algebraic {

namespace internal {


template<class T>
struct base {
    using value_type = T;

  protected:
    value_type _value;

  public:
    base(const value_type& value = {}) noexcept(NO_EXCEPT) : _value(value) {};

    inline operator value_type() const noexcept(NO_EXCEPT) { return this->_value; }
    inline value_type val() const noexcept(NO_EXCEPT) { return this->_value; };

    inline const value_type* operator->() const noexcept(NO_EXCEPT) { return &this->_value; };
    inline value_type* operator->() noexcept(NO_EXCEPT) { return &this->_value; };

    inline bool operator==(const base& other) const noexcept(NO_EXCEPT) { return this->_value == other._value; };
    inline bool operator!=(const base& other) const noexcept(NO_EXCEPT) { return this->_value != other._value; };
};


struct commutative {};

struct invertible {};


struct magma {};

struct semigroup : magma {};

struct monoid : semigroup {};

struct group : monoid, invertible {};


} // namespace internal

} // namespace algebraic

} // namespace lib

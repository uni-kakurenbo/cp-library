#pragma once


#include <utility>
#include <concepts>

#include "internal/dev_env.hpp"


namespace lib {

namespace algebraic {


template<class T>
struct base {
    using value_type = T;

  protected:
    value_type _value;

  public:
    template<class... Args>
        requires std::constructible_from<value_type, Args...>
    base(Args&&... args) noexcept(NO_EXCEPT) : _value(std::forward<Args>(args)...) {}

    inline operator value_type() const noexcept(NO_EXCEPT) { return this->_value; }
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

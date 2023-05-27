#pragma once

#include "internal/dev_env.hpp"
#include "algebraic/internal/base.hpp"

namespace lib {

namespace algebraic {


template<class T> struct addition : internal::base<T>, internal::group, internal::commutative {
    using internal::base<T>::base;
    addition() noexcept(NO_EXCEPT) : internal::base<T>() {};
    friend inline addition operator+(const addition& lhs, const addition& rhs) noexcept(NO_EXCEPT) { return lhs.val() + rhs.val(); }
    inline addition operator-() const noexcept(NO_EXCEPT) { return -this->val(); }
};


} // namespace algebraic

} // namespace lib

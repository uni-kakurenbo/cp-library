#pragma once

#include "algebraic/internal/base.hpp"

namespace lib {

namespace algebraic {


template<class T> struct addition : internal::base<T>, internal::group, internal::commutative {
    using internal::base<T>::base;
    addition() : internal::base<T>() {};
    friend inline addition operator+(const addition& lhs, const addition& rhs) { return lhs.val() + rhs.val(); }
    inline addition operator-() const { return -this->val(); }
};


} // namespace algebraic

} // namespace lib

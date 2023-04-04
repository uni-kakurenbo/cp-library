#pragma once

#include "algebraic/internal/base.hpp"
#include "algebraic/internal/commutative.hpp"

namespace lib {

namespace algebraic {


template<class T> struct addition : internal::base<T>, internal::group, internal::commutative {
    using internal::base<T>::base;
    addition() : internal::base<T>() {};
    inline addition operator+(const addition& other) const { return this->val() + other.val(); }
    inline addition operator-() const { return -this->val(); }
};


} // namespace algebraic

} // namespace lib

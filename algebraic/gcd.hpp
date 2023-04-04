#pragma once

#include <numeric>

#include "algebraic/internal/base.hpp"
#include "algebraic/internal/commutative.hpp"


namespace lib {

namespace algebraic {


template<class T> struct gcd : internal::base<T>, internal::monoid, internal::commutative {
    using internal::base<T>::base;
    gcd() : internal::base<T>() {};
    inline gcd operator+(const gcd& other) const { return std::gcd(this->val(), other.val()); }
};


} // namespace algebraic

} // namespace lib

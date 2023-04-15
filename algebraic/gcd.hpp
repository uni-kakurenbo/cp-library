#pragma once

#include <numeric>

#include "algebraic/internal/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct gcd : internal::base<T>, internal::monoid, internal::commutative {
    using internal::base<T>::base;
    gcd() : internal::base<T>() {};
    friend inline gcd operator+(const gcd& lhs, const gcd& rhs) { return std::gcd(lhs.val(), rhs.val()); }
};


} // namespace algebraic

} // namespace lib

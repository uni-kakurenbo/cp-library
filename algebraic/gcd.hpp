#pragma once

#include <numeric>

#include "internal/dev_env.hpp"
#include "algebraic/internal/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct gcd : internal::base<T>, internal::monoid, internal::commutative {
    using internal::base<T>::base;
    gcd() noexcept(NO_EXCEPT) : internal::base<T>() {};
    friend inline gcd operator+(const gcd& lhs, const gcd& rhs) noexcept(NO_EXCEPT) { return std::gcd(lhs.val(), rhs.val()); }
};


} // namespace algebraic

} // namespace lib

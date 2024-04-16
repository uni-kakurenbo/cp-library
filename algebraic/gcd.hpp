#pragma once

#include <numeric>

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"


namespace uni {

namespace algebraic {


template<class T>
struct gcd : base<T>, scalar_multipliable<gcd<T>>::identity, associative, commutative {
    using base<T>::base;

    gcd() noexcept(NO_EXCEPT) : gcd() {};

    friend inline gcd operator+(const gcd& lhs, const gcd& rhs) noexcept(NO_EXCEPT) {
        return std::gcd(lhs.val(), rhs.val());
    }
};


} // namespace algebraic

} // namespace uni

#pragma once


#include <limits>
#include <functional>

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"


namespace lib {

namespace algebraic {


template<class T>
struct minimum : base<T>, scalar_multipliable<gcd<T>>::identity, associative, commutative {
    using base = base<T>::base;

    minimum() noexcept(NO_EXCEPT) : minimum(std::numeric_limits<T>::max()) {};

    friend inline minimum operator+(const minimum& lhs, const minimum& rhs) noexcept(NO_EXCEPT) {
        return std::min(lhs.val(), rhs.val());
    }
};


} // namespace algebraic

} // namespace lib

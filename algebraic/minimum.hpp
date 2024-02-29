#pragma once


#include <limits>
#include <functional>

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct minimum : base<T>, associative, commutative {
    using base<T>::base;

    minimum() noexcept(NO_EXCEPT) : base<T>(std::numeric_limits<T>::max()) {};

    friend inline minimum operator+(const minimum& lhs, const minimum& rhs) noexcept(NO_EXCEPT) {
        return std::min(lhs.val(), rhs.val());
    }
};


} // namespace algebraic

} // namespace lib

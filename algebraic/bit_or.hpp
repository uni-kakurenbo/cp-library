#pragma once

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"

namespace lib {

namespace algebraic {


template<class T>
struct bit_or : base<T>, scalar_multipliable<bit_or<T>>::identity, associative, commutative {
    using base<T>::base;

    friend inline bit_or operator+(const bit_or& lhs, const bit_or& rhs) noexcept(NO_EXCEPT) {
        return lhs.val() bitor rhs.val();
    }
};


} // namespace algebraic

} // namespace lib

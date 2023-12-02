#pragma once

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"
#include "numeric/limits.hpp"

namespace lib {

namespace algebraic {


template<class T>
struct bit_and : base<T>, associative, commutative {
    using base<T>::base;
    bit_and() noexcept(NO_EXCEPT) : base<T>(lib::numeric_limits<T>::max()) {};
    friend inline bit_and operator+(const bit_and& lhs, const bit_and& rhs) noexcept(NO_EXCEPT) { return lhs.val() bitand rhs.val(); }
};


} // namespace algebraic

} // namespace lib

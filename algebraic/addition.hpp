#pragma once


#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"
#include "algebraic/internal/concepts.hpp"


namespace lib {

namespace algebraic {


template<class T>
struct addition : base<T>, associative, commutative {
    using base<T>::base;

    friend inline addition operator+(const addition& lhs, const addition& rhs) noexcept(NO_EXCEPT) {
        return lhs.val() + rhs.val();
    }

    template<std::integral Scalar>
    friend inline addition operator*(const Scalar k, const addition& rhs) noexcept(NO_EXCEPT) {
        return k * rhs.val();
    }

    inline addition operator-() const noexcept(NO_EXCEPT)
        requires internal::invertible<T>
    {
        return -this->val();
    }
};


} // namespace algebraic

} // namespace lib

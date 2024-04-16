#pragma once

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"

namespace uni {

namespace algebraic {


template<class T>
struct bit_xor : base<T>, associative, commutative {
    using base<T>::base;

    friend inline bit_xor operator+(const bit_xor& lhs, const bit_xor& rhs) noexcept(NO_EXCEPT) {
        return lhs.val() xor rhs.val();
    }

    template<std::integral Scalar>
    friend inline bit_xor operator*(const Scalar k, const bit_xor& val) noexcept(NO_EXCEPT) {
        return k % 2 == 0 ? bit_xor{} : val;
    }

    inline bit_xor operator-() const noexcept(NO_EXCEPT) {
        return this->val();
    }
};


} // namespace algebraic

} // namespace uni

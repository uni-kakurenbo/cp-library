#pragma once

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"

namespace lib {

namespace algebraic {


template<class T>
struct bit_xor : base<T>, associative, commutative {
    using base<T>::base;
    bit_xor() noexcept(NO_EXCEPT) : base<T>() {};
    friend inline bit_xor operator+(const bit_xor& lhs, const bit_xor& rhs) noexcept(NO_EXCEPT) { return lhs.val() xor rhs.val(); }
    inline bit_xor operator-() const noexcept(NO_EXCEPT) { return this->val(); }
};


} // namespace algebraic

} // namespace lib

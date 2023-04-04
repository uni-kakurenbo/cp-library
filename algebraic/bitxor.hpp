#pragma once

#include "algebraic/internal/base.hpp"
#include "algebraic/internal/commutative.hpp"

namespace lib {

namespace algebraic {


template<class T> struct bitxor : internal::base<T>, internal::group, internal::commutative {
    using internal::base<T>::base;
    bitxor() : internal::base<T>() {};
    friend inline bitxor operator+(const bitxor& lhs, const bitxor& rhs) { return lhs.val() xor rhs.val(); }
    inline bitxor operator-() const { return this->val(); }
};


} // namespace algebraic

} // namespace lib

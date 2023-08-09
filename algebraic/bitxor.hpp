#pragma once

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"

namespace lib {

namespace algebraic {


template<class T> struct bitxor : base<T>, group, commutative {
    using base<T>::base;
    bitxor() noexcept(NO_EXCEPT) : base<T>() {};
    friend inline bitxor operator+(const bitxor& lhs, const bitxor& rhs) noexcept(NO_EXCEPT) { return lhs.val() xor rhs.val(); }
    inline bitxor operator-() const noexcept(NO_EXCEPT) { return this->val(); }
};


} // namespace algebraic

} // namespace lib

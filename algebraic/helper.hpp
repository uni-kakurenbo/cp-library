#pragma once


#include "algebraic/base.hpp"

namespace lib {

namespace algebraic {


template<class T, T (*op)(T, T), T (*e)(), class... Tags>
struct helper : lib::algebraic::base<T>, lib::algebraic::scalar_multipliable<helper<T, op, e, Tags...>>::automatic, Tags... {
    using lib::algebraic::base<T>::base;

    helper() : helper(e()) {}

    friend inline helper operator+(const helper& lhs, const helper& rhs) noexcept(NO_EXCEPT) {
        return op(lhs.val(), rhs.val());
    }
};


template<class T, T (*op)(T, T), T (*e)()>
using monoid_helper = helper<T, op, e, associative>;


} // namespace algebraic

} // namespace lib

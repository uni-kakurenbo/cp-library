#pragma once


#include "algebraic/base.hpp"

namespace lib {

namespace algebraic {


template<class T, T (*op)(T, T), T (*e)(), class... Tags>
struct helper : lib::algebraic::base<T>, Tags... {
    using base = lib::algebraic::base<T>;
    using base::base;

    helper() : base(e()) {}

    friend inline helper operator+(const helper& lhs, const helper& rhs) noexcept(NO_EXCEPT) {
        return op(lhs.val(), rhs.val());
    }
};


template<class T, T (*op)(T, T), T (*e)()>
using monoid_helper = helper<T, op, e, associative>;


} // namespace algebraic

} // namespace lib

#pragma once


#include "algebraic/base.hpp"
#include "algebraic/null.hpp"
#include "algebraic/internal/concepts.hpp"


namespace uni {

namespace algebraic {


template<class T, T (*op)(T, T), T (*e)(), class... Tags>
struct helper : uni::algebraic::base<T>, uni::algebraic::scalar_multipliable<helper<T, op, e, Tags...>>::automatic, Tags... {
    using uni::algebraic::base<T>::base;

    helper() : helper(e()) {}

    friend inline helper operator+(const helper& lhs, const helper& rhs) noexcept(NO_EXCEPT) {
        return op(lhs.val(), rhs.val());
    }
};


template<class T, T (*op)(T, T), T (*e)()>
using monoid_helper = helper<T, op, e, associative>;


template<class T>
struct make_magma {
    using type = null<T>;

    static_assert(internal::magma<type>);
};


template<internal::magma T>
struct make_magma<T> {
    using type = T;
};

template<class T>
using make_magma_t = typename make_magma<T>::type;


} // namespace algebraic

} // namespace uni

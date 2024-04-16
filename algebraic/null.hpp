#pragma once


#include <cstddef>


#include "internal/dev_env.hpp"
#include "internal/dummy.hpp"

#include "algebraic/base.hpp"
#include "algebraic/internal/concepts.hpp"


namespace uni {

namespace algebraic {


template<class T = uni::internal::dummy>
struct null : base<T>, scalar_multipliable<null<T>>::identity, associative, commutative {
    using base<T>::base;

    friend inline null operator+(const null& lhs, const null& rhs) noexcept(NO_EXCEPT) {
        if(lhs == null{}) return rhs;
        return lhs;
    }

    inline null operator-() const noexcept(NO_EXCEPT)
        requires internal::invertible<T>
    {
        return -*this;
    }
};


} // namespace algebraic

} // namespace uni

#pragma once

#include <utility>

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"

namespace uni {

namespace algebraic {


template<class T, bool REVERSE = false>
struct affine : base<std::pair<T,T>>, scalar_multipliable<affine<T, REVERSE>>::automatic, associative {
    using base<std::pair<T,T>>::base;

    affine() noexcept(NO_EXCEPT) : affine({ 1, 0 }) {};

    friend inline affine operator+(const affine& lhs, const affine& rhs) noexcept(NO_EXCEPT) {
        if constexpr(REVERSE) return affine({ rhs->first * lhs->first, rhs->first * lhs->second + rhs->second });
        return affine({ lhs->first * rhs->first, lhs->first * rhs->second + lhs->second });
    }
};


} // namespace algebraic

} // namespace uni

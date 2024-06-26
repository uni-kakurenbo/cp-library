#pragma once

#include <utility>

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"

namespace uni {

namespace algebraic {


template<class T, bool REVERSE = false>
struct affine : base<std::pair<T, T>>, scalar_multipliable<affine<T, REVERSE>>::automatic, associative {
    using base<std::pair<T, T>>::base;

    affine() noexcept(NO_EXCEPT) : affine({ 1, 0 }) {};

    friend inline affine operator+(const affine& lhs, const affine& rhs) noexcept(NO_EXCEPT) {
        if constexpr(REVERSE) return affine({ lhs->first * rhs->first, lhs(rhs->second) });
        return affine({ rhs->first * lhs->first, rhs(lhs->second) });
    }

    inline auto operator()(const T& x) const noexcept(NO_EXCEPT) {
        return this->val().first * x + this->val().second;
    }
};


} // namespace algebraic

} // namespace uni

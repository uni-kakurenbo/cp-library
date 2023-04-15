#pragma once

#include <utility>

#include "algebraic/internal/base.hpp"

namespace lib {

namespace algebraic {


template<class T, bool REVERSE = false> struct affine : internal::base<std::pair<T,T>>, internal::monoid {
    using internal::base<std::pair<T,T>>::base;
    affine() : internal::base<std::pair<T,T>>({ 1, 0 }) {};
    friend inline affine operator+(const affine& lhs, const affine& rhs) {
        if constexpr(REVERSE) return affine({ rhs->first * lhs->first, rhs->first * lhs->second + rhs->second });
        return affine({ lhs->first * rhs->first, lhs->first * rhs->second + lhs->second });
    }
};


} // namespace algebraic

} // namespace lib

#pragma once

#include <utility>

#include "algebraic/internal/base.hpp"

namespace lib {

namespace algebraic {


template<class T, bool REVERSE = false> struct affine : internal::base<std::pair<T,T>>, internal::monoid {
    using internal::base<std::pair<T,T>>::base;
    affine() : internal::base<std::pair<T,T>>({ 1, 0 }) {};
    inline affine operator+(const affine& other) const {
        if constexpr(REVERSE) return affine({ other->first * (*this)->first, other->first * (*this)->second + other->second });
        return affine({ (*this)->first * other->first, (*this)->first * other->second + (*this)->second });
    }
};


} // namespace algebraic

} // namespace lib

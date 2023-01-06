#pragma once


#include <utility>

#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoids {


template<class T, bool REVERSE = false> struct affine : base<std::pair<T,T>> {
    using base<std::pair<T,T>>::base;
    affine() : base<std::pair<T,T>>({ 1, 0 }) {};
    inline affine operator*(const affine& other) const {
        if constexpr(REVERSE) return affine({ other->first * (*this)->first, other->first * (*this)->second + other->second });
        return affine({ (*this)->first * other->first, (*this)->first * other->second + (*this)->second });
    }
};


} // namespace monoids

} // namespace lib

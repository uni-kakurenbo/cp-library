#pragma once


#include <utility>

#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoid {


template<class T> struct affine : base<std::pair<T,T>> {
    using base<std::pair<T,T>>::base;
    affine() : base<std::pair<T,T>>({ 1, 0 }) {};
    inline affine operator*(const affine& other) const { return affine({ (*this)->first * other->first, (*this)->second * other->first + other->second }); }
};


} // namespace monoid

} // namespace lib

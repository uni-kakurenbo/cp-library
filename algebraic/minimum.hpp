#pragma once


#include <limits>
#include <functional>

#include "algebraic/internal/base.hpp"
#include "algebraic/internal/commutative.hpp"


namespace lib {

namespace algebraic {


template<class T> struct minimum : internal::base<T>, internal::monoid, internal::commutativeommutative {
    using internal::base<T>::base;
    minimum() : internal::base<T>(std::numeric_limits<T>::max() / 2 - 1) {};
    inline minimum operator+(const minimum& other) const { return std::min(this->val(), other.val()); }
};


} // namespace algebraic

} // namespace lib

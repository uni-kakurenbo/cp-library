#pragma once


#include <limits>
#include <functional>

#include "algebraic/internal/base.hpp"
#include "algebraic/internal/commutative.hpp"


namespace lib {

namespace algebraic {


template<class T> struct maximum : internal::base<T>, internal::monoid, internal::commutative {
    using internal::base<T>::base;
    maximum() : internal::base<T>(std::numeric_limits<T>::lowest() / 2 + 1) {};
    inline maximum operator+(const maximum& other) const { return std::max(this->val(), other.val()); }
};


} // namespace algebraic

} // namespace lib

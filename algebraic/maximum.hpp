#pragma once


#include <limits>
#include <functional>

#include "algebraic/internal/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct maximum : internal::base<T>, internal::monoid, internal::commutative {
    using internal::base<T>::base;
    maximum() : internal::base<T>(std::numeric_limits<T>::lowest()) {};
    friend inline maximum operator+(const maximum& lhs, const maximum& rhs) { return std::max(lhs.val(), rhs.val()); }
};


} // namespace algebraic

} // namespace lib

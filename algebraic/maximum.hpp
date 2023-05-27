#pragma once


#include <limits>
#include <functional>

#include "internal/dev_env.hpp"
#include "algebraic/internal/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct maximum : internal::base<T>, internal::monoid, internal::commutative {
    using internal::base<T>::base;
    maximum() noexcept(NO_EXCEPT) : internal::base<T>(std::numeric_limits<T>::lowest()) {};
    friend inline maximum operator+(const maximum& lhs, const maximum& rhs) noexcept(NO_EXCEPT) { return std::max(lhs.val(), rhs.val()); }
};


} // namespace algebraic

} // namespace lib

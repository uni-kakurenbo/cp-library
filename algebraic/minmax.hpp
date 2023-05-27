#pragma once


#include <utility>
#include <limits>
#include <functional>

#include "internal/dev_env.hpp"
#include "algebraic/internal/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct minmax : internal::base<std::pair<T,T>>, internal::monoid, internal::commutative {
    using internal::base<std::pair<T,T>>::base;
    minmax() noexcept(NO_EXCEPT) : internal::base<std::pair<T,T>>({ std::numeric_limits<T>::max(), std::numeric_limits<T>::lowest() }) {};
    friend inline minmax operator+(const minmax& lhs, const minmax& rhs) noexcept(NO_EXCEPT) { return minmax({ std::min(lhs.val().first, rhs->first), std::max(lhs.val().second, rhs->second) }); }
};


} // namespace algebraic

} // namespace lib

#pragma once


#include <utility>
#include <limits>
#include <functional>

#include "algebraic/internal/base.hpp"
#include "algebraic/internal/commutative.hpp"


namespace lib {

namespace algebraic {


template<class T> struct minmax : internal::base<std::pair<T,T>>, internal::monoid, internal::commutative {
    using internal::base<std::pair<T,T>>::base;
    minmax() : internal::base<std::pair<T,T>>({ std::numeric_limits<T>::max() / 2 - 1, std::numeric_limits<T>::lowest() / 2 + 1}) {};
    inline minmax operator+(const minmax& other) const { return minmax({ std::min(this->val().first, other->first), std::max(this->val().second, other->second) }); }
};


} // namespace algebraic

} // namespace lib

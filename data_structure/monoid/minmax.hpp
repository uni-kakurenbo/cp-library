#pragma once


#include <utility>
#include <limits>
#include <functional>

#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoids {


template<class T> struct minmax : base<std::pair<T,T>> {
    using base<std::pair<T,T>>::base;
    minmax() : base<std::pair<T,T>>({ std::numeric_limits<T>::max(), std::numeric_limits<T>::min() }) {};
    inline minmax operator*(const minmax& other) const { return minmax({ std::min(this->val().first, other->first), std::max(this->val().second, other->second) }); }
};


} // namespace monoids

} // namespace lib

#pragma once


#include <limits>
#include <functional>

#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoids {


template<class T> struct minimum : base<T> {
    using base<T>::base;
    minimum() : base<T>(std::numeric_limits<T>::max()) {};
    inline minimum operator*(const minimum& other) const { return std::min(this->val(), other.val()); }
};


} // namespace monoids

} // namespace lib

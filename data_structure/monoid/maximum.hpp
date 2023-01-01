#pragma once


#include <limits>
#include <functional>

#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoid {


template<class T> struct maximum : base<T> {
    using base<T>::base;
    maximum() : base<T>(std::numeric_limits<T>::min()) {};
    inline maximum operator*(const maximum& other) const { return std::max(this->val(), other.val()); }
};


} // namespace monoid

} // namespace lib

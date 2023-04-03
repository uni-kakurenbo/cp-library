#pragma once

#include <numeric>

#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoids {


template<class T> struct gcd : base<T> {
    using base<T>::base;
    gcd() : base<T>() {};
    inline gcd operator*(const gcd& other) const { return std::gcd(this->val(), other.val()); }
};


} // namespace monoids

} // namespace lib

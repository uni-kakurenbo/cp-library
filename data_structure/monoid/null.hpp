#pragma once


#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoids {


template<class T> struct null : base<T> {
    using base<T>::base;
    inline null operator*(const null&) const { return *this; }
};


} // namespace monoids

} // namespace lib

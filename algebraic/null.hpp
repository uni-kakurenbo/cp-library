#pragma once


#include "algebraic/internal/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct null : internal::base<T>, internal::magma {
    using internal::base<T>::base;
    inline null operator+(const null&) const { return *this; }
};


} // namespace algebraic

} // namespace lib

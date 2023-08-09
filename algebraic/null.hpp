#pragma once


#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"


namespace lib {

namespace algebraic {


template<class T> struct null : base<T>, internal::magma {
    using base<T>::base;
    inline null operator+(const null&) const noexcept(NO_EXCEPT) { return *this; }
};


} // namespace algebraic

} // namespace lib

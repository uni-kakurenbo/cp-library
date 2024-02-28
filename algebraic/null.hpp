#pragma once


#include <cstddef>


#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"


namespace lib {

namespace algebraic {


template<class T = std::nullptr_t> struct null : base<T>, associative, commutative {
    using base<T>::base;
    inline null operator+(const null& x) const noexcept(NO_EXCEPT) {
        if(x == null{}) return *this;
        return x;
    }
};


} // namespace algebraic

} // namespace lib

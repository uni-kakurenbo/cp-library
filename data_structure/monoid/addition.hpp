#pragma once


#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoid {


template<class T> struct addition : base<T> {
    using base<T>::base;
    addition() : base<T>() {};
    inline addition operator*(const addition& other) const { return this->val() + other.val(); }
};


} // namespace monoid

} // namespace lib

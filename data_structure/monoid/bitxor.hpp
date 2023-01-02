#pragma once


#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoids {


template<class T> struct bitxor : base<T> {
    using base<T>::base;
    bitxor() : base<T>() {};
    inline bitxor operator*(const bitxor& other) const { return this->val() xor other.val(); }
};


} // namespace monoids

} // namespace lib

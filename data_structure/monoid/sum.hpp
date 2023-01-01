#pragma once


#include "data_structure/monoid/base.hpp"


namespace lib {

namespace monoid {


template<class T> struct sum : base<T> {
    using value_type = typename base<T>::value_type;
    using size_type = internal::size_t;

  protected:
    size_type _length = 1;

  public:
    sum() : base<T>() {};
    sum(const value_type& value, const size_type& length = 1) : base<T>(value), _length(length) {};

    inline size_type len() const { return this->_length; }

    inline sum operator*(const sum& other) const {
        return sum(this->val() + other.val(), this->len() + other.len());
    }
};


} // namespace monoid

} // namespace lib

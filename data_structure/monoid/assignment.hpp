#pragma once


#include <optional>

#include "data_structure/semigroup/assignment.hpp"


namespace lib {

namespace monoids {


template<class T> struct assignment : base<std::optional<T>> {
    using base<std::optional<T>>::base;
    assignment() : base<std::optional<T>>() {};
    inline assignment operator*(const assignment& other) const {
        if(other->has_value()) return other.val();
        return *this;
    }
};


} // namespace monoids

} // namespace lib

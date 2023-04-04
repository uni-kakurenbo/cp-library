#pragma once

#include <optional>

#include "algebraic/internal/base.hpp"
#include "algebraic/internal/commutative.hpp"

namespace lib {

namespace algebraic {


template<class T> struct assignment : internal::base<std::optional<T>>, internal::monoid, internal::commutative {
    using internal::base<std::optional<T>>::base;
    assignment() : internal::base<std::optional<T>>() {};
    inline assignment operator+(const assignment& other) const {
        if(other->has_value()) return other.val();
        return *this;
    }
};


} // namespace algebraic

} // namespace lib

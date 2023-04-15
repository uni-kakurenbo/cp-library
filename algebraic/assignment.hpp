#pragma once

#include <optional>

#include "algebraic/internal/base.hpp"

namespace lib {

namespace algebraic {


template<class T> struct assignment : internal::base<std::optional<T>>, internal::monoid, internal::commutative {
    using internal::base<std::optional<T>>::base;
    assignment() : internal::base<std::optional<T>>() {};
    friend inline assignment operator+(const assignment& lhs, const assignment& rhs) {
        if(rhs->has_value()) return rhs.val();
        return lhs;
    }
};


} // namespace algebraic

} // namespace lib

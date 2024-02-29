#pragma once

#include <optional>

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"

namespace lib {

namespace algebraic {


template<class T> struct assignment : base<std::optional<T>>, associative {
    using base<std::optional<T>>::base;

    assignment() noexcept(NO_EXCEPT) : base<std::optional<T>>() {};

    friend inline assignment operator+(const assignment& lhs, const assignment& rhs) noexcept(NO_EXCEPT) {
        if(lhs->has_value()) return lhs;
        return rhs;
    }
};


} // namespace algebraic

} // namespace lib

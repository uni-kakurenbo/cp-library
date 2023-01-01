#pragma once

#include <cstddef>

#include "internal/types.hpp"


namespace lib {

namespace action {

namespace internal {

struct base {};

} // namespace internal



template<class operator_monoid = std::nullptr_t> struct base : internal::base {
    static operator_monoid compose(const operator_monoid& x, const lib::internal::size_t) { return x; }
};


} // namespace action

} // namespace lib

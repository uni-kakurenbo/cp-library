#pragma once

#include <cstddef>

#include "internal/types.hpp"


namespace lib {

namespace actions {

namespace internal {

struct base {};

} // namespace internal



template<class operation = std::nullptr_t> struct base : internal::base {
    static operation fold(const operation& x, const lib::internal::size_t) { return x; }
};


} // namespace actions

} // namespace lib

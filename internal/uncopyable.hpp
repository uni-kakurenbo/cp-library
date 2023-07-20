#pragma once


#include "internal/dev_env.hpp"


namespace lib {

namespace internal {


struct uncopyable {
    uncopyable() noexcept(NO_EXCEPT) {}
    uncopyable(const uncopyable&) = delete;
    uncopyable& operator=(const uncopyable&) = delete;
};


} // namespace internal

} // namespace lib

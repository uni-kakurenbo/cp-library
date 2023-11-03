#pragma once

#include <utility>
#include "internal/dev_env.hpp"

#define ALIAS_TEMPLATE_FUNCTION(wrapping, wrapped) template<class... Args> inline auto wrapping(Args&&... args) noexcept(NO_EXCEPT) { return wrapped(std::forward<Args>(args)...); }

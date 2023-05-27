#pragma once

#include <utility>
#include "internal/dev_env.hpp"

#define ALIAS_TEMPLATE_FUNCTION(wrapping, wrapped) template<class... Args> inline auto wrapping(Args&&... args) noexcept(DEV_ENV) { return wrapped(std::forward<Args>(args)...); }

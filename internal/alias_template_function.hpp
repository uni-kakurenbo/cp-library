#pragma once

#include <utility>

#define ALIAS_TEMPLATE_FUNCTION(wrapping, wrapped) template<class... Args> inline auto wrapping(Args&&... args) { return wrapped(std::forward<Args>(args)...); }

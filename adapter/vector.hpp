#pragma once


#include <vector>
#include <algorithm>
#include "adapter/internal/container_extender.hpp"


namespace lib {


template<class... Args>
using vector = internal::extended_container<std::vector<Args...>>;


} // namespace lib

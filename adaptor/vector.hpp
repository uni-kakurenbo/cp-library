#pragma once


#include <vector>
#include <algorithm>


#include "adaptor/internal/container_extender.hpp"


namespace uni {


template<class... Args>
using vector = internal::extended_container<std::vector<Args...>>;


} // namespace uni

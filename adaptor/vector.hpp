#pragma once


#include <vector>
#include <algorithm>


#include "adaptor/internal/advanced_container.hpp"


namespace uni {


template<class... Args>
using vector = internal::advanced_container<std::vector<Args...>>;


} // namespace uni

#pragma once


#include <array>
#include <algorithm>


#include "adaptor/internal/advanced_container.hpp"


namespace uni {


template<class T, int N>
using array = internal::advanced_container<std::array<T,N>>;


} // namespace uni

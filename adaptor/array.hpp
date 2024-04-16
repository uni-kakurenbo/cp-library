#pragma once


#include <array>
#include <algorithm>


#include "adaptor/internal/container_extender.hpp"


namespace uni {


template<class T, int N>
using array = internal::extended_container<std::array<T,N>>;


} // namespace uni

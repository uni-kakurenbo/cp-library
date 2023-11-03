#pragma once


#include <vector>
#include <algorithm>
#include "adapter/internal/sequence.hpp"


namespace lib {


template<class... Args>
using vector = internal::extended_sequence<std::vector<Args...>>;


} // namespace lib

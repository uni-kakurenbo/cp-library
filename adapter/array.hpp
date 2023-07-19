#pragma once


#include <array>
#include <algorithm>
#include "adapter/internal/sequence.hpp"


namespace lib {


template<class... Args>
using array = internal::extended_sequence<std::array<Args...>>;


} // namespace lib

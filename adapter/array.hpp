#pragma once


#include <array>
#include <algorithm>
#include "adapter/internal/sequence.hpp"


namespace lib {


template<class T, int N>
using array = internal::extended_sequence<std::array<T,N>>;


} // namespace lib

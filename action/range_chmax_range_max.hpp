#pragma once


#include "algebraic/maximum.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_chmax_range_max = amplifier<algebraic::maximum<T>>;


} // namespace actions

} // namespace uni

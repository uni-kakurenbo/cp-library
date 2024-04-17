#pragma once


#include "algebraic/maximum.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_chmin_range_min = amplifier<algebraic::maximum<T>>;


} // namespace actions

} // namespace uni

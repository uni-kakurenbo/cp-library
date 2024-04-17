#pragma once


#include "algebraic/minmax.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_chminchmax_range_minmax = amplifier<algebraic::minmax<T>>;


} // namespace actions

} // namespace uni

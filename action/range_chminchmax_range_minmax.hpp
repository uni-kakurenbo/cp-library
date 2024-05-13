#pragma once


#include "algebraic/minmax.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_chminchmax_range_minmax = amplifier<algebraic::minmax<T>>;

static_assert(internal::full_action<range_chminchmax_range_minmax<int>>);


} // namespace actions

} // namespace uni

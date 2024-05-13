#pragma once


#include "algebraic/maximum.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_chmin_range_min = amplifier<algebraic::maximum<T>>;

static_assert(internal::full_action<range_chmin_range_min<int>>);


} // namespace actions

} // namespace uni

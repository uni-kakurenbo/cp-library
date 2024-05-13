#pragma once


#include "algebraic/gcd.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_chgcd_range_gcd = amplifier<uni::algebraic::gcd<T>>;

static_assert(internal::full_action<range_chgcd_range_gcd<int>>);


} // namespace actions

} // namespace uni

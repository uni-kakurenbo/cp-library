#pragma once


#include "algebraic/gcd.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_chgcd_range_gcd = make_operatable_t<uni::algebraic::gcd<T>>;


} // namespace actions

} // namespace uni

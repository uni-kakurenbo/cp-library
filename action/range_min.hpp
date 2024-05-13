#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/minimum.hpp"



namespace uni {

namespace actions {


template<class T>
using range_min = make_operatable_t<uni::algebraic::minimum<T>>;

static_assert(internal::operand_only_action<range_min<int>>);


} // namespace actions

} // namespace uni

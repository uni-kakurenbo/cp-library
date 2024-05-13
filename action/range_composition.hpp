#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/affine.hpp"



namespace uni {

namespace actions {


template<class T, bool REVERSE = false>
using range_composition = make_operatable_t<uni::algebraic::affine<T, REVERSE>>;

static_assert(internal::operand_only_action<range_composition<int>>);


} // namespace actions

} // namespace uni

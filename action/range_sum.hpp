#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/addition.hpp"


namespace uni {

namespace actions {


template<class T>
using range_sum = make_operatable_t<uni::algebraic::addition<T>>;

static_assert(internal::operand_only_action<range_sum<int>>);


} // namespace actions

} // namespace uni

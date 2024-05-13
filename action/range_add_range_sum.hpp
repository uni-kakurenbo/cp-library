#pragma once


#include "internal/dev_env.hpp"

#include "algebraic/addition.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_add_range_sum = amplifier<algebraic::addition<T>>;

static_assert(internal::full_action<range_add_range_sum<int>>);


} // namespace actions

} // namespace uni

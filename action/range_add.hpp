#pragma once


#include "action/range_sum.hpp"
#include "action/helpers.hpp"

#include "algebraic/addition.hpp"


namespace uni {

namespace actions {


template<class T>
using range_add = uni::actions::make_effective_t<uni::algebraic::addition<T>>;


} // namesapce actions

} // namespace uni

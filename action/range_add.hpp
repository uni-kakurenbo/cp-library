#pragma once


#include "action/range_sum.hpp"
#include "action/helpers.hpp"

#include "algebraic/addition.hpp"


namespace lib {

namespace actions {


template<class T>
using range_add = lib::actions::make_effective_t<lib::algebraic::addition<T>>;


} // namesapce actions

} // namespace lib

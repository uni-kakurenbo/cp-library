#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/maximum.hpp"



namespace uni {

namespace actions {


template<class T>
using range_max = make_operatable_t<uni::algebraic::maximum<T>>;


} // namespace actions

} // namespace uni

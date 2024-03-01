#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/maximum.hpp"



namespace lib {

namespace actions {


template<class T>
using range_max = make_operatable_t<lib::algebraic::maximum<T>>;


} // namespace actions

} // namespace lib

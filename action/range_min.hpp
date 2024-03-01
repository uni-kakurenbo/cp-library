#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/minimum.hpp"



namespace lib {

namespace actions {


template<class T>
using range_max = make_operatable_t<lib::algebraic::minimum<T>>;


} // namespace actions

} // namespace lib

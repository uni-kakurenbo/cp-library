#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/addition.hpp"


namespace lib {

namespace actions {


template<class T>
using range_sum = make_operatable_t<lib::algebraic::addition<T>>;


} // namespace actions

} // namespace lib

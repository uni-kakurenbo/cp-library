#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/bit_xor.hpp"



namespace lib {

namespace actions {


template<class T>
using range_bitxor = make_operatable_t<lib::algebraic::bit_xor<T>>;


} // namespace actions

} // namespace lib

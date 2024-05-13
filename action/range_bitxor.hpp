#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/bit_xor.hpp"



namespace uni {

namespace actions {


template<class T>
using range_bitxor = make_operatable_t<uni::algebraic::bit_xor<T>>;

static_assert(internal::operand_only_action<range_bitxor<int>>);



} // namespace actions

} // namespace uni

#pragma once


#include "internal/dev_env.hpp"

#include "algebraic/bit_xor.hpp"

#include "action/helpers.hpp"


namespace uni {

namespace actions {


template<class T>
using range_flip_range_bitxor = amplifier<uni::algebraic::bit_xor<T>>;

static_assert(internal::full_action<range_flip_range_bitxor<int>>);


} // namespace actions

} // namespace uni

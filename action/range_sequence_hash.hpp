#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/rolling_hash.hpp"



namespace uni {

namespace actions {


template<
    bool REVERSE = false,
    uni::internal::modint_family T = uni::static_modint_64bit<(1UL << 61) - 1>,
    typename T::value_type BASE = 0
>
using range_sequence_hash = make_operatable_t<uni::algebraic::rolling_hash<REVERSE, T, BASE>>;

static_assert(internal::operand_only_action<range_sequence_hash<false>>);
static_assert(internal::operand_only_action<range_sequence_hash<true>>);


} // namespace actions

} // namespace uni

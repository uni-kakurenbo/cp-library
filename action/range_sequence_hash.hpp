#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/rolling_hash.hpp"



namespace lib {

namespace actions {


template<
    bool REVERSE = false,
    lib::internal::modint_family T = lib::static_modint_64bit<(1UL << 61) - 1>,
    typename T::value_type BASE = 0
>
using range_sequence_hash = make_operatable_t<lib::algebraic::rolling_hash<T>>;


} // namespace actions

} // namespace lib

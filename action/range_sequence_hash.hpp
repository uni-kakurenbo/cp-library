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
struct range_sequence_hash : base<> {
    using operand = algebraic::rolling_hash<REVERSE, T, BASE>;
};


} // namespace actions

} // namespace lib

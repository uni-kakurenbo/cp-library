#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/bit_xor.hpp"



namespace lib {

namespace actions {


template<class T>
struct range_bitxor : base<> {
    using operand = algebraic::bit_xor<T>;
};

} // namespace actions

} // namespace lib

#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/bitxor.hpp"



namespace lib {

namespace actions {


template<class T> struct range_bitxor : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::bitxor<T>;
};

} // namespace actions

} // namespace lib

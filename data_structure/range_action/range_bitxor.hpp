#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/bitxor.hpp"


namespace lib {

namespace actions {


template<class T> struct range_bitxor : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::bitxor<T>;

    static operand rev(const operand& x, const operand& y) { return x * y; }
};


} // namespace actions

} // namespace lib

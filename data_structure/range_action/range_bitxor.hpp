#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/bitxor.hpp"


namespace lib {

namespace actions {


template<class T> struct range_bitxor : base<> {
    static constexpr flags tags{ flags::fenwick_tree, flags::segment_tree };

    using operand_monoid = monoids::bitxor<T>;

    static operand_monoid rev(const operand_monoid& x, const operand_monoid& y) { return x * y; }
};


} // namespace actions


} // namespace lib

#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/bitxor.hpp"


namespace lib {

namespace actions {


template<class T> struct range_bitxor : base<> {
    static constexpr flags tags{ flags::fenwick_tree, flags::segment_tree, flags::disjoint_sparse_table };

    using operand = monoids::bitxor<T>;

    static operand rev(const operand& x, const operand& y) { return x * y; }
};


} // namespace actions

} // namespace lib

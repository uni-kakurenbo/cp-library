#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/gcd.hpp"


namespace lib {

namespace actions {


template<class T> struct range_gcd : base<> {
    static constexpr flags tags{ flags::segment_tree, flags::disjoint_sparse_table };

    using operand = monoids::gcd<T>;
};


} // namespace actions

} // namespace lib

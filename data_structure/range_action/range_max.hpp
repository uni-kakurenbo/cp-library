#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/maximum.hpp"


namespace lib {

namespace actions {


template<class T> struct range_max : base<> {
    static constexpr flags tags{ flags::segment_tree, flags::disjoint_sparse_table };

    using operand = monoids::maximum<T>;
};


} // namespace actions


template<class T> struct segment_tree<actions::range_max<T>> : internal::segment_tree_lib::core<monoids::maximum<T>> {
    using internal::segment_tree_lib::core<monoids::maximum<T>>::core;

    inline auto max(const size_t first, const size_t last) { return this->fold(first, last); }
    inline auto max() { return this->fold(); }
};


} // namespace lib

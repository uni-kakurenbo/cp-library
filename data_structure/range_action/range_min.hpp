#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/minimum.hpp"


namespace lib {

namespace actions {


template<class T> struct range_min : base<> {
    static constexpr flags tags{ flags::segment_tree, flags::disjoint_sparse_table };

    using operand = monoids::minimum<T>;
};


} // namespace actions


template<class T> struct segment_tree<actions::range_min<T>> : internal::segment_tree_lib::core<monoids::minimum<T>> {
    using internal::segment_tree_lib::core<monoids::minimum<T>>::core;

    inline auto min(const size_t first, const size_t last) { return this->fold(first, last); }
    inline auto min() { return this->fold(); }
};


} // namespace lib

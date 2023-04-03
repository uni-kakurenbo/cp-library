#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/minimum.hpp"
#include "data_structure/monoid/assignment.hpp"


namespace lib {

namespace actions {


template<class T> struct range_set_range_min : base<monoids::assignment<T>> {
    static constexpr flags tags{ flags::implicit_treap, flags::lazy_segment_tree };

    using operand = monoids::minimum<T>;
    using operation = monoids::assignment<T>;

    static operand map(const operand& x, const operation& y) { return y->value_or(x.val()); }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_set_range_min<T>> : internal::lazy_segment_tree_lib::core<actions::range_set_range_min<T>> {
    using internal::lazy_segment_tree_lib::core<actions::range_set_range_min<T>>::core;

    inline auto set(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto set(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto set(const T& val) { return this->apply(val); }

    inline auto min(const size_t first, const size_t last) { return this->fold(first, last); }
    inline auto min() { return this->fold(); }
};


} // namespace lib

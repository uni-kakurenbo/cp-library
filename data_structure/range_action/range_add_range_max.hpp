#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/minmax.hpp"
#include "data_structure/monoid/addition.hpp"


namespace lib {

namespace actions {


template<class T> struct range_add_range_max : base<monoids::addition<T>> {
    static constexpr flags tags{ flags::implicit_treap, flags::lazy_segment_tree };

    using operand_monoid = monoids::maximum<T>;
    using operator_monoid = monoids::addition<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return x.val() + y.val(); }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_add_range_max<T>> : internal::lazy_segment_tree_lib::core<actions::range_add_range_max<T>> {
    using internal::lazy_segment_tree_lib::core<actions::range_add_range_max<T>>::core;

    inline auto add(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto add(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto add(const T& val) { return this->apply(val); }

    inline auto max(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto max() { return this->prod(); }
};


} // namespace lib

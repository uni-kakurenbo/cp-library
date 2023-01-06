#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/addition.hpp"
#include "data_structure/monoid/assignment.hpp"


namespace lib {

namespace actions {


template<class T> struct range_set_range_sum : base<> {
    static constexpr flags tags{ flags::implicit_treap, flags::lazy_segment_tree };

    using operand_monoid = monoids::addition<T>;
    using operator_monoid = monoids::assignment<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return y->value_or(x.val()); }
    static operator_monoid fold(const operator_monoid& x, const lib::internal::size_t length) {
        if(x->has_value()) return operator_monoid(x->operator*() * length);
        return x;
    }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_set_range_sum<T>> : internal::lazy_segment_tree_lib::core<actions::range_set_range_sum<T>> {
    using internal::lazy_segment_tree_lib::core<actions::range_set_range_sum<T>>::core;

    inline auto set(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto set(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto set(const T& val) { return this->apply(val); }

    inline auto sum(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto sum() { return this->prod(); }
};


} // namespace lib

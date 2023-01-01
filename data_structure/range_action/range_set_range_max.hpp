#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"

#include "data_structure/monoid/maximum.hpp"
#include "data_structure/monoid/assignment.hpp"


namespace lib {

namespace action {


template<class T> struct range_set_range_max : base<monoid::assignment<T>>  {
    using operand_monoid = monoid::maximum<T>;
    using operator_monoid = monoid::assignment<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return y->value_or(x.val()); }
};


} // namespace action


template<class T> struct implicit_treap<action::range_set_range_max<T>> : implicit_treap_lib::core<action::range_set_range_max<T>> {
    using implicit_treap_lib::core<action::range_set_range_max<T>>::core;

    inline auto set(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto set(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto set(const T& val) { return this->apply(val); }

    inline auto max(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto max() { return this->prod(); }
};


} // namespace lib

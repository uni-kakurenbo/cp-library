#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"

#include "data_structure/monoid/addition.hpp"


namespace lib {

namespace action {


template<class T> struct range_add_range_sum : base<> {
    using operand_monoid = monoid::addition<T>;
    using operator_monoid = monoid::addition<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return x.val() + y.val(); }
    static operator_monoid compose(const operator_monoid& x, const lib::internal::size_t length) { return x.val() * length; }
};


} // namespace action


template<class T> struct implicit_treap<action::range_add_range_sum<T>> : implicit_treap_lib::core<action::range_add_range_sum<T>> {
    using implicit_treap_lib::core<action::range_add_range_sum<T>>::core;

    inline auto add(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto add(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto add(const T& val) { return this->apply(val); }

    inline auto sum(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto sum() { return this->prod(); }
};


} // namespace lib

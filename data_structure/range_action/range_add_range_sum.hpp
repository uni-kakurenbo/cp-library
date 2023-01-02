#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/addition.hpp"


namespace lib {

namespace actions {


template<class T> struct range_add_range_sum : base<> {
    static constexpr flags tags{ flags::implicit_treap };

    using operand_monoid = monoids::addition<T>;
    using operator_monoid = monoids::addition<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return x.val() + y.val(); }
    static operator_monoid compose(const operator_monoid& x, const lib::internal::size_t length) { return x.val() * length; }
};


} // namespace actions


template<class T> struct implicit_treap<actions::range_add_range_sum<T>> : internal::implicit_treap_lib::core<actions::range_add_range_sum<T>> {
    using internal::implicit_treap_lib::core<actions::range_add_range_sum<T>>::core;

    inline auto add(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto add(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto add(const T& val) { return this->apply(val); }

    inline auto sum(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto sum() { return this->prod(); }
};


} // namespace lib

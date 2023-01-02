#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/minimum.hpp"
#include "data_structure/monoid/assignment.hpp"


namespace lib {

namespace actions {


template<class T> struct range_set_range_min : base<monoids::assignment<T>> {
    static constexpr flags tags{ flags::implicit_treap };

    using operand_monoid = monoids::minimum<T>;
    using operator_monoid = monoids::assignment<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return y->value_or(x.val()); }
};


} // namespace actions



template<class T> struct implicit_treap<actions::range_set_range_min<T>> : internal::implicit_treap_lib::core<actions::range_set_range_min<T>> {
    using internal::implicit_treap_lib::core<actions::range_set_range_min<T>>::core;

    inline auto set(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto set(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto set(const T& val) { return this->apply(val); }

    inline auto min(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto min() { return this->prod(); }
};


} // namespace lib

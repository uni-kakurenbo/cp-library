#pragma once

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/addition.hpp"



namespace lib {

namespace actions {


template<class T> struct range_sum : base<> {
    static constexpr flags tags{ flags::fenwick_tree };

    using operand_monoid = monoids::addition<T>;

    static operand_monoid rev(const operand_monoid& x, const operand_monoid& y) { return x.val() - y.val(); }
};


} // namespace actions


template<class T> struct fenwick_tree<actions::range_sum<T>> : internal::fenwick_tree_lib::core<actions::range_sum<T>> {
    using internal::fenwick_tree_lib::core<actions::range_sum<T>>::core;

    inline auto add(const size_t pos, const T& val) { return this->apply(pos, val); }

    inline auto sum(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto sum() { return this->prod(); }
};


} // namespace lib

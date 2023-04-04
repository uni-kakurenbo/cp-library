#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/assignment.hpp"


namespace lib {

namespace actions {


template<class T> struct range_set_range_sum : base<> {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::addition<T>;
    using operation = algebraic::assignment<T>;

    static operand map(const operand& x, const operation& y) { return y->value_or(x.val()); }
    static operation fold(const operation& x, const lib::internal::size_t length) {
        if(x->has_value()) return operation(x->operator*() * length);
        return x;
    }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_set_range_sum<T>> : internal::lazy_segment_tree_impl::core<actions::range_set_range_sum<T>> {
    using internal::lazy_segment_tree_impl::core<actions::range_set_range_sum<T>>::core;

    inline auto set(const size_t first, const size_t last, const T& val) { return this->apply(first, last, val); }
    inline auto set(const size_t pos, const T& val) { return this->apply(pos, val); }
    inline auto set(const T& val) { return this->apply(val); }

    inline auto sum(const size_t first, const size_t last) { return this->fold(first, last); }
    inline auto sum() { return this->fold(); }
};


} // namespace lib

#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/addition.hpp"
#include "data_structure/monoid/affine.hpp"


namespace lib {

namespace actions {


template<class T> struct range_affine_range_sum : base<> {
    static constexpr flags tags{ flags::implicit_treap, flags::lazy_segment_tree };

    using operand_monoid = monoids::addition<T>;
    using operator_monoid = monoids::affine<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return y->first * x.val() + y->second; }
    static operator_monoid fold(const operator_monoid& x, const lib::internal::size_t length) { return operator_monoid({ x->first, x->second * length }); }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_affine_range_sum<T>> : internal::lazy_segment_tree_lib::core<actions::range_affine_range_sum<T>> {
    using internal::lazy_segment_tree_lib::core<actions::range_affine_range_sum<T>>::core;


    inline auto affine(const size_t first, const size_t last, const T& a, const T& b) { return this->apply(first, last, { a, b }); }
    inline auto affine(const size_t pos, const T& a, const T& b) { return this->apply(pos, { a, b }); }
    inline auto affine(const T& a, const T& b) { return this->apply({ a, b }); }

    inline auto set(const size_t first, const size_t last, const T& val) { return this->apply(first, last, { 0, val }); }
    inline auto set(const size_t pos, const T& val) { return this->apply(pos, { 0, val }); }
    inline auto set(const T& val) { return this->apply({ 0, val }); }

    inline auto add(const size_t first, const size_t last, const T& val) { return this->apply(first, last, { 1, val }); }
    inline auto add(const size_t pos, const T& val) { return this->apply(pos, { 1, val }); }
    inline auto add(const T& val) { return this->apply({ 1, val }); }

    inline auto times(const size_t first, const size_t last, const T& val) { return this->apply(first, last, { val, 0 }); }
    inline auto times(const size_t pos, const T& val) { return this->apply(pos, { val, 0 }); }
    inline auto times(const T& val) { return this->apply({ val, 0 }); }


    inline auto sum(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto sum() { return this->prod(); }
};


} // namespace lib

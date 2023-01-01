#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"

#include "data_structure/monoid/addition.hpp"
#include "data_structure/monoid/affine.hpp"


namespace lib {

namespace action {


template<class T> struct range_affine_range_sum : base<> {
    using operand_monoid = monoid::addition<T>;
    using operator_monoid = monoid::affine<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) { return y->first * x.val() + y->second; }
    static operator_monoid compose(const operator_monoid& x, const lib::internal::size_t length) { return operator_monoid({ x->first, x->second * length }); }
};


} // namespace action


template<class T> struct implicit_treap<action::range_affine_range_sum<T>> : implicit_treap_lib::core<action::range_affine_range_sum<T>> {
    using implicit_treap_lib::core<action::range_affine_range_sum<T>>::core;


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

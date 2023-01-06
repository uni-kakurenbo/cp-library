#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "data_structure/monoid/minmax.hpp"
#include "data_structure/monoid/affine.hpp"


namespace lib {


namespace actions {


template<class T> struct range_affine_range_minmax : base<monoids::affine<T>> {
    static constexpr flags tags{ flags::implicit_treap, flags::lazy_segment_tree };

    using operand_monoid = monoids::minmax<T>;
    using operator_monoid = monoids::affine<T>;

    static operand_monoid map(const operand_monoid& x, const operator_monoid& y) {
        auto res = operand_monoid({ x->first * y->first + y->second, x->second * y->first + y->second });
        if (y->first < 0) std::swap(res->first, res->second);
        return res;
    }
};


} // namespace actions


template<class T> struct lazy_segment_tree<actions::range_affine_range_minmax<T>> : internal::lazy_segment_tree_lib::core<actions::range_affine_range_minmax<T>> {
    using internal::lazy_segment_tree_lib::core<actions::range_affine_range_minmax<T>>::core;


    inline auto set(const size_t first, const size_t last, const T& val) { return this->apply(first, last, { 0, val }); }
    inline auto set(const size_t pos, const T& val) { return this->apply(pos, { 0, val }); }
    inline auto set(const T& val) { return this->apply({ 0, val }); }

    inline auto add(const size_t first, const size_t last, const T& val) { return this->apply(first, last, { 1, val }); }
    inline auto add(const size_t pos, const T& val) { return this->apply(pos, { 1, val }); }
    inline auto add(const T& val) { return this->apply({ 1, val }); }

    inline auto times(const size_t first, const size_t last, const T& val) { return this->apply(first, last, { val, 0 }); }
    inline auto times(const size_t pos, const T& val) { return this->apply(pos, { val, 0 }); }
    inline auto times(const T& val) { return this->apply({ val, 0 }); }


    inline auto minmax(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto minmax() { return this->prod(); }

    inline auto min(const size_t first, const size_t last) { return this->prod(first, last).first; }
    inline auto min() { return this->prod().first; }

    inline auto max(const size_t first, const size_t last) { return this->prod(first, last).second; }
    inline auto max() { return this->prod().second; }
};


} // namespace lib

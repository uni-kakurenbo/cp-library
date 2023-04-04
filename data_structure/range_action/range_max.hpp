#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/maximum.hpp"


namespace lib {

namespace actions {


template<class T> struct range_max : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::maximum<T>;
};


} // namespace actions


template<class T> struct segment_tree<actions::range_max<T>> : internal::segment_tree_impl::core<algebraic::maximum<T>> {
    using internal::segment_tree_impl::core<algebraic::maximum<T>>::core;

    inline auto max(const size_t first, const size_t last) { return this->fold(first, last); }
    inline auto max() { return this->fold(); }
};


} // namespace lib

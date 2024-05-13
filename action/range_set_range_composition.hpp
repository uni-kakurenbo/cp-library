#pragma once


#include <utility>


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/affine.hpp"
#include "algebraic/assignment.hpp"



namespace uni {

namespace actions {


template<class T, bool REVERSE = false>
struct range_set_range_composition : base<algebraic::assignment<std::pair<T, T>>> {
    using base<algebraic::assignment<std::pair<T, T>>>::base;

    using operand = algebraic::affine<T, REVERSE>;
    using operation = algebraic::assignment<std::pair<T, T>>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f->value_or(std::make_pair(x->first, x->second));
    }
};

static_assert(internal::full_action<range_set_range_composition<int>>);


} // namespace actions

} // namespace uni

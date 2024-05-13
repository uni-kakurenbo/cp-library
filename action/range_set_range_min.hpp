#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/minimum.hpp"
#include "algebraic/assignment.hpp"


namespace uni {

namespace actions {


template<class T>
struct range_set_range_min : base<algebraic::assignment<T>> {
    using operand = algebraic::minimum<T>;
    using operation = algebraic::assignment<T>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f->value_or(x.val());
    }
};

static_assert(internal::full_action<range_set_range_min<int>>);


} // namespace actions

} // namespace uni

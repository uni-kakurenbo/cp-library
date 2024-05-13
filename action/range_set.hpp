#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/null.hpp"
#include "algebraic/assignment.hpp"


namespace uni {

namespace actions {


template<class T>
struct range_set : base<algebraic::assignment<T>> {
    using operand = algebraic::null<T>;
    using operation = algebraic::assignment<T>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f->value_or(x.val());
    }
};

static_assert(internal::full_action<range_set<int>>);


} // namesapce actions

} // namespace uni

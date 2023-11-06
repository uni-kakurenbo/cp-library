#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/maximum.hpp"
#include "algebraic/assignment.hpp"


namespace lib {

namespace actions {


template<class T> struct range_set_range_max : base<algebraic::assignment<T>>  {
    using operand = algebraic::maximum<T>;
    using operation = algebraic::assignment<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) { return y->value_or(x.val()); }
};


} // namespace actions

} // namespace lib

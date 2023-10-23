#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/maximum.hpp"
#include "algebraic/assignment.hpp"


namespace lib {

namespace actions {


template<class T> struct range_set_range_max : base<algebraic::assignment<T>>  {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::maximum<T>;
    using operation = algebraic::assignment<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) { return y->value_or(x.val()); }
};


} // namespace actions

} // namespace lib

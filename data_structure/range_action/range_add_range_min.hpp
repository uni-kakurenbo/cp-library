#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/minimum.hpp"
#include "algebraic/addition.hpp"


namespace lib {

namespace actions {


template<class T> struct range_add_range_min : base<algebraic::addition<T>> {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::minimum<T>;
    using operation = algebraic::addition<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) { return x.val() + y.val(); }
};


} // namespace actions

} // namespace lib

#pragma once


#include "internal/dev_env.hpp"
#include "internal/exception.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/null.hpp"


namespace lib {

namespace actions {


template<class T> struct null : base<algebraic::null<T>> {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::addition<T>;
    using operation = algebraic::null<T>;

    static operand map(const operand& x, const operation&) noexcept(NO_EXCEPT) { return x; }
};


} // namespace actions

} // namespace lib

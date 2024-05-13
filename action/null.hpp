#pragma once


#include "internal/dev_env.hpp"
#include "internal/exception.hpp"

#include "action/base.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/null.hpp"


namespace uni {

namespace actions {


template<class T>
struct null : base<algebraic::null<T>> {
    using operand = algebraic::null<T>;
    using operation = algebraic::null<T>;

    static operand mapping(const operation&, const operand& x) noexcept(NO_EXCEPT) { return x; }
};


} // namespace actions

} // namespace uni

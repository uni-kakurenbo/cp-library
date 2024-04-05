#pragma once


#include "action/range_sum.hpp"
#include "action/helpers.hpp"

#include "algebraic/null.hpp"
#include "algebraic/assignment.hpp"


namespace lib {

namespace actions {


template<class T>
struct range_set {
    using operand = algebraic::null<T>;
    using operation = algebraic::assignment<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        return y->value_or(x.val());
    }

    static operation fold(const operation& x, const lib::internal::size_t) noexcept(NO_EXCEPT) {
        if(x->has_value()) return operation(x->operator*());
        return x;
    }
};


} // namesapce actions

} // namespace lib

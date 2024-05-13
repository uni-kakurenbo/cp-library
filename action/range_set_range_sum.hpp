#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/assignment.hpp"


namespace uni {

namespace actions {


template<class T>
struct range_set_range_sum {
    using operand = algebraic::addition<T>;
    using operation = algebraic::assignment<T>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f->value_or(x.val());
    }

    static auto power(const operation& x, const uni::internal::size_t length) noexcept(NO_EXCEPT) {
        if(x->has_value()) return operation(x->operator*() * length);
        return x;
    }
};

static_assert(internal::full_action<range_set_range_sum<int>>);


} // namespace actions

} // namespace uni

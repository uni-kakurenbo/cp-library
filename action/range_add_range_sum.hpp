#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/addition.hpp"


namespace uni {

namespace actions {


template<class T>
struct range_add_range_sum {
    using operand = algebraic::addition<T>;
    using operation = algebraic::addition<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        return x.val() + y.val();
    }

    static operation fold(const operation& x, const uni::internal::size_t length) noexcept(NO_EXCEPT) {
        return x.val() * length;
    }
};


} // namespace actions

} // namespace uni

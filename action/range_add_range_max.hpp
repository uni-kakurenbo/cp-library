#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/maximum.hpp"
#include "algebraic/addition.hpp"



namespace lib {

namespace actions {


template<class T>
struct range_add_range_max : base<algebraic::addition<T>> {
    using operand = algebraic::maximum<T>;
    using operation = algebraic::addition<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        return x.val() + y.val();
    }
};


} // namespace actions

} // namespace lib

#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/maximum.hpp"
#include "algebraic/addition.hpp"



namespace uni {

namespace actions {


template<class T>
struct range_add_range_max : base<algebraic::addition<T>> {
    using operand = algebraic::maximum<T>;
    using operation = algebraic::addition<T>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f.val() + x.val();
    }
};


static_assert(internal::full_action<uni::actions::range_add_range_max<int>>);


} // namespace actions

} // namespace uni

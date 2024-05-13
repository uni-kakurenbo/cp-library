#pragma once


#include "action/base.hpp"
#include "action/helpers.hpp"

#include "algebraic/addition.hpp"


namespace uni {

namespace actions {



template<class T>
struct range_add : base<algebraic::addition<T>> {
    using operand = algebraic::null<T>;
    using operation = algebraic::addition<T>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f.val() + x.val();
    }
};

static_assert(internal::full_action<range_add<int>>);


} // namesapce actions

} // namespace uni

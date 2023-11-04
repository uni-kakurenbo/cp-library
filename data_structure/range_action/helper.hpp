#pragma once

#include <set>
#include "algebraic/helper.hpp"
#include "data_structure/range_action/base.hpp"


namespace lib {

namespace actions {

template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)(), F (*folding)(F, int) = nullptr>
struct helper : lib::actions::base<algebraic::monoid_helper<F,composition,id>> {
    using operand = algebraic::monoid_helper<S,op,e>;
    using operation = algebraic::monoid_helper<F,composition,id>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        return mapping(y.val(), x.val());
    }
    static operation fold(const operation& x, [[maybe_unused]] const lib::internal::size_t length) noexcept(NO_EXCEPT) {
        if constexpr(folding == nullptr) return x;
        return folding(x.val(), length);
    }
};


} // namespace actions

} // namespace lib

#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/affine.hpp"



namespace lib {

namespace actions {


template<class T, bool REVERSE = false>
struct range_affine_range_sum {
    using operand = algebraic::addition<T>;
    using operation = algebraic::affine<T, REVERSE>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        return y->first * x.val() + y->second;
    }

    static operation fold(const operation& x, const lib::internal::size_t length) noexcept(NO_EXCEPT) {
        return operation({ x->first, x->second * length });
    }
};


} // namespace actions

} // namespace lib

#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/affine.hpp"



namespace uni {

namespace actions {


template<class T, bool REVERSE = false>
struct range_affine_range_sum {
    using operand = algebraic::addition<T>;
    using operation = algebraic::affine<T, !REVERSE>;

    static operand mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        return f(x.val());
    }

    static auto power(const operation& f, const uni::internal::size_t length) noexcept(NO_EXCEPT) {
        return operation({ f->first, f->second * length });
    }
};

static_assert(internal::full_action<range_affine_range_sum<int>>);


} // namespace actions

} // namespace uni

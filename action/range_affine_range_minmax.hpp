#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/minmax.hpp"
#include "algebraic/affine.hpp"



namespace uni {


namespace actions {


template<class T, bool REVERSE = false>
struct range_affine_range_minmax : base<algebraic::affine<T, !REVERSE>> {
    using operand = algebraic::minmax<T>;
    using operation = algebraic::affine<T, !REVERSE>;

    static auto mapping(const operation& f, const operand& x) noexcept(NO_EXCEPT) {
        auto res = operand({ f(x->first), f(x->second) });
        if (f->first < 0) std::swap(res->first, res->second);
        return res;
    }
};

static_assert(internal::full_action<range_affine_range_minmax<int>>);


} // namespace actions

} // namespace uni

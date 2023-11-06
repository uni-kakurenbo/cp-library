#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/minmax.hpp"
#include "algebraic/affine.hpp"



namespace lib {


namespace actions {


template<class T> struct range_affine_range_minmax : base<algebraic::affine<T>> {
    using operand = algebraic::minmax<T>;
    using operation = algebraic::affine<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        auto res = operand({ x->first * y->first + y->second, x->second * y->first + y->second });
        if (y->first < 0) std::swap(res->first, res->second);
        return res;
    }
};


} // namespace actions

} // namespace lib

#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/addition.hpp"
#include "algebraic/affine.hpp"



namespace lib {

namespace actions {


template<class T> struct range_affine_range_sum : base<> {
    static constexpr flags tags{ flags::range_folding, flags::range_operation };

    using operand = algebraic::addition<T>;
    using operation = algebraic::affine<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) { return y->first * x.val() + y->second; }
    static operation fold(const operation& x, const lib::internal::size_t length) noexcept(NO_EXCEPT) { return operation({ x->first, x->second * length }); }
};


} // namespace actions

} // namespace lib

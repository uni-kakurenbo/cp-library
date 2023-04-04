#pragma once


#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/gcd.hpp"


namespace lib {

namespace actions {


template<class T> struct range_gcd : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::gcd<T>;
};


} // namespace actions

} // namespace lib

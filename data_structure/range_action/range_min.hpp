#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/internal/declarations.hpp"

#include "data_structure/range_action/base.hpp"
#include "data_structure/range_action/flags.hpp"

#include "algebraic/minimum.hpp"



namespace lib {

namespace actions {


template<class T> struct range_min : base<> {
    static constexpr flags tags{ flags::range_folding };

    using operand = algebraic::minimum<T>;
};


} // namespace actions

} // namespace lib

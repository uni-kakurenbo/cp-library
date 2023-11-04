#pragma once


#include "internal/dev_env.hpp"

#include "data_structure/range_action/base.hpp"

#include "algebraic/gcd.hpp"



namespace lib {

namespace actions {


template<class T> struct range_gcd : base<> {
    using operand = algebraic::gcd<T>;
};


} // namespace actions

} // namespace lib

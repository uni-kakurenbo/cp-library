#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/minimum.hpp"



namespace lib {

namespace actions {


template<class T> struct range_min : base<> {
    using operand = algebraic::minimum<T>;
};


} // namespace actions

} // namespace lib

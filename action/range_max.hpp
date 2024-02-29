#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/maximum.hpp"



namespace lib {

namespace actions {


template<class T>
struct range_max : base<> {
    using operand = algebraic::maximum<T>;
};


} // namespace actions

} // namespace lib

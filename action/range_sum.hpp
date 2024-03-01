#pragma once


#include "internal/dev_env.hpp"

#include "action/base.hpp"

#include "algebraic/addition.hpp"


namespace lib {

namespace actions {


template<class T>
struct range_sum : base<> {
    using operand = algebraic::addition<T>;
};


} // namespace actions

} // namespace lib
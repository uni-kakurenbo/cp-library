#pragma once


#include <cassert>
#include <vector>

#include "internal/dev_env.hpp"

#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "data_structure/range_action/range_sum.hpp"
#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace actions {

template<class T> struct range_add {
    static constexpr flags tags{ flags::range_operation };
};

} // namespace actions

} // namespace lib

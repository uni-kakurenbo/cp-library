#pragma once

#include <cstdint>

namespace lib {


enum class comp : std::int8_t {
    equal_to,
    not_equal_to,

    equals = equal_to,
    eq = equal_to,

    under,
    over,
    under_or,
    over_or,

    less = under,
    more = over,

    less_than = under,
    more_than = over,
    not_less_than = over_or,
    not_more_than = under_or,

    leq = under_or,
    geq = over_or
}; // namespace comp

enum class interval : std::int8_t {
    right_open,
    left_open,
    open,
    closed,
}; // namespace comp


} // namespace lib

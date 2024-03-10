#pragma once


#include <cstdint>
#include <utility>


#include "snippet/aliases.hpp"

#include "internal/type_traits.hpp"

#include "numeric/limits.hpp"


namespace lib {


enum class comparison : std::uint8_t {
    equal_to,
    not_equal_to,

    equals = equal_to,
    eq = equal_to,

    under,
    over,
    or_under,
    or_over,

    less = under,
    more = over,

    less_than = under,
    more_than = over,
    not_less_than = or_over,
    not_more_than = or_under,

    leq = or_under,
    geq = or_over
};


enum class interval_notation : std::uint8_t {
    right_open,
    left_open,
    open,
    closed,
};


enum class replacement_policy : std::uint8_t {
    insert_sync,
    overwrite_sync,
    overwrite_async
};


enum class rotation : std::int8_t {
    clockwise,

    counter_clockwise,
    anti_clockwise = counter_clockwise,
};


enum class positional_relation : std::int8_t {
    clockwise,

    counter_clockwise,
    anti_clockwise = counter_clockwise,

    straight_backward,
    straight_forward,
    straight_middle,

    in,
    on,
    out
};


enum class alignment : std::int8_t {
    left,
    center,
    right
};


enum class tree_indexing_policy : std::int8_t {
    implicit_key,
    sorted,
};


} // namespace lib

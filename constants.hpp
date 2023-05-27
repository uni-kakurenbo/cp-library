#pragma once


#include <cstdint>
#include <utility>
#include <limits>

#include "snippet/aliases.hpp"


namespace lib {


i32 INF32 = (std::numeric_limits<i32>::max() >> 1) - 1;
i64 INF64 = (std::numeric_limits<i64>::max() >> 1) - 1;

constexpr char LN = '\n';
constexpr char SPC = ' ';

constexpr std::pair<int,int> DIRS4[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
constexpr std::pair<int,int> DIRS8[] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };


enum class comp : std::int8_t {
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
}; // namespace comp

enum class interval : std::int8_t {
    right_open,
    left_open,
    open,
    closed,
}; // namespace comp


} // namespace lib

#pragma once


#include <cstdint>
#include <utility>

#include "snippet/aliases.hpp"

#include "numeric/limits.hpp"


namespace lib {


i32 INF32 = numeric_limits<i32>::arithmetic_infinity();
i64 INF64 = numeric_limits<i64>::arithmetic_infinity();

template<class T>
constexpr T INF = numeric_limits<T>::arithmetic_infinity();


constexpr char LN = '\n';
constexpr char SPC = ' ';

constexpr std::pair<int,int> DIRS4[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
constexpr std::pair<int,int> DIRS8[] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };


enum class comp : std::uint8_t {
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

enum class interval : std::uint8_t {
    right_open,
    left_open,
    open,
    closed,
};

enum class replacing_policy : std::uint8_t {
    insert_sync,
    overwrite_sync,
    overwrite_async
};


} // namespace lib

#pragma once


#include "macro/basic.hpp"
#include "macro/loop.hpp"

#define EXPAND_TAILS_IMPL(base, tail) UNPAREN base UNPAREN tail

#define EXPAND_TAILS_AUX_1(base, tails) EXPAND_TAILS_IMPL(base, tails)
#define EXPAND_TAILS_AUX_2(base, tails, ...) EXPAND_TAILS_IMPL(base, tails) EXPAND_TAILS_AUX_1(base, __VA_ARGS__)
#define EXPAND_TAILS_AUX_3(base, tails, ...) EXPAND_TAILS_IMPL(base, tails) EXPAND_TAILS_AUX_2(base, __VA_ARGS__)
#define EXPAND_TAILS_AUX_4(base, tails, ...) EXPAND_TAILS_IMPL(base, tails) EXPAND_TAILS_AUX_3(base, __VA_ARGS__)
#define EXPAND_TAILS_AUX_5(base, tails, ...) EXPAND_TAILS_IMPL(base, tails) EXPAND_TAILS_AUX_4(base, __VA_ARGS__)
#define EXPAND_TAILS_AUX_6(base, tails, ...) EXPAND_TAILS_IMPL(base, tails) EXPAND_TAILS_AUX_5(base, __VA_ARGS__)
#define EXPAND_TAILS_AUX_7(base, tails, ...) EXPAND_TAILS_IMPL(base, tails) EXPAND_TAILS_AUX_6(base, __VA_ARGS__)
#define EXPAND_TAILS_AUX_8(base, tails, ...) EXPAND_TAILS_IMPL(base, tails) EXPAND_TAILS_AUX_7(base, __VA_ARGS__)

#define EXPAND_TAILS(base, ...) CONCAT(EXPAND_TAILS_AUX_, VA_ARGS_SIZE(__VA_ARGS__))(base, __VA_ARGS__)

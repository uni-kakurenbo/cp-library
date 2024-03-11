#pragma once

#include "debugger/debug.hpp"

#ifdef LOCAL_JUDGE

#define DEBUGGER_ENABLED 1

#define debug(...) debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define debug_(...) do { debugger::raw(nullptr, "\033[3;35m#" + std::to_string(__LINE__) + "\033[m  "); debugger::raw(__VA_ARGS__); debugger::raw(nullptr, "\033[m\n"); } while(0);
#define DEBUG if constexpr(true)

#else

#define debug(...) ({ ; })
#define debug_(...) ({ ; })
#define DEBUG if constexpr(false)

#endif

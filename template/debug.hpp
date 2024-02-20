#pragma once

#ifdef LOCAL_JUDGE

#define DEBUGGER_ENABLED 1
#include <debugger/debug.hpp>
#define debug(...) debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define _debug(...) do { debugger::DEBUG(nullptr, "\033[3;35m#" + std::to_string(__LINE__) + "\033[m  "); debugger::DEBUG(__VA_ARGS__); debugger::DEBUG(nullptr, "\033[m\n"); } while(0);
#define DEBUG if constexpr(true)

#else

#define debug(...) ({ ; })
#define _debug(...) ({ ; })
#define DEBUG if constexpr(false)

#endif

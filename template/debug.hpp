#pragma once


#ifdef LOCAL_JUDGE

#define DEBUGGER_ENABLED
#define DEBUGGER_COLORED_OUTPUT 1

#endif

#include <string>
#include "debugger/debug.hpp"


#ifdef DEBUGGER_ENABLED


#define debug(...) debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __FILE__, __VA_ARGS__)
#define debug_(...) do { const std::string file = __FILE__; debugger::raw(nullptr, debugger::COLOR_LINE + file.substr(file.find_last_of("/") + 1) + " #" + std::to_string(__LINE__) + debugger::COLOR_INIT + "  "); debugger::raw(__VA_ARGS__); debugger::raw(nullptr, debugger::COLOR_INIT + "\n"); } while(0);
#define DEBUG if constexpr(true)


#else


#define debug(...) ({ ; })
#define debug_(...) ({ ; })
#define DEBUG if constexpr(false)


#endif

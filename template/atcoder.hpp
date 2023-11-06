#pragma once

#ifdef LOCAL_JUDGE
    #define DEBUGGER_ENABLED 1
    #include <debug.hpp>
    #define debug(...) debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
    #define DEBUG(...) do { debugger::DEBUG(nullptr, "\033[3;35m#" + std::to_string(__LINE__) + "\033[m  "); debugger::DEBUG(__VA_ARGS__); debugger::DEBUG(nullptr, "\033[m\n"); } while(0);
#else
    #define debug(...) ({ ; })
    #define DEBUG(...) ({ ; })
#endif

#include "include/template.hpp"

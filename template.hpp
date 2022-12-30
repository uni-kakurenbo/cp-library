#pragma once

#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "snippet/fast_io.hpp"

#ifdef LOCAL_JUDGE
    #include<debug>
    #define debug(...) debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
    #define DEBUG(...) do { debugger::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); debugger::DEBUG(__VA_ARGS__); debugger::DEBUG(nullptr, "\033[m\n"); } while(0);
#else
    #define debug(...) ({ ; })
    #define DEBUG(...) ({ ; })
#endif

#include "input.hpp"
#include "output.hpp"

input_adapter _input;
output_adapter _print;

#define input _input
#define print _print

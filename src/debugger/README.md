# C++ Debugger for Competitive Programming

For use:
```C++
#include <debug>
#define debug(...) debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
```

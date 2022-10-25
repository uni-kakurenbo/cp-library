# C++ Debugger for Competition Programming

For use:
```C++
#include <debug>
#define debug(...) Debug::debug(Debug::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
```

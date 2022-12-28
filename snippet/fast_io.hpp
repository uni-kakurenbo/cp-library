#pragma once

#include <iostream>

#ifdef __GNUC__

__attribute__((constructor)) inline void fast_io() { std::ios::sync_with_stdio(false), std::cin.tie(nullptr); }

#else

inline void fast_io() { std::ios::sync_with_stdio(false), std::cin.tie(nullptr); }

#endif

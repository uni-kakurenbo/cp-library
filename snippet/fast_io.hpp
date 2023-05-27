#pragma once


#include <iostream>

#include "internal/dev_env.hpp"


#ifdef __GNUC__

__attribute__((constructor)) inline void fast_io() noexcept(DEV_ENV) { std::ios::sync_with_stdio(false), std::cin.tie(nullptr); }

#else

inline void fast_io() noexcept(DEV_ENV) { std::ios::sync_with_stdio(false), std::cin.tie(nullptr); }

#endif

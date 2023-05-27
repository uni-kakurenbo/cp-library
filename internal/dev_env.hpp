#pragma once

#ifdef LOCAL_JUDGE
    constexpr bool DEV_ENV = true;
    constexpr bool NO_EXCEPT = false;
#else
    constexpr bool DEV_ENV = false;
    constexpr bool NO_EXCEPT = true;
#endif // LOCAL_JUDGE

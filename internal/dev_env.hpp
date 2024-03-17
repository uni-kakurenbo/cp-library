#pragma once


#ifdef LOCAL_JUDGE
    inline constexpr bool DEV_ENV = true;
    inline constexpr bool NO_EXCEPT = false;
#else
    inline constexpr bool DEV_ENV = false;
    inline constexpr bool NO_EXCEPT = true;
#endif // LOCAL_JUDGE


#if __cplusplus >= 202100L
    #define CPP20 true
    #define CPP23 true
#elif __cplusplus >= 202002L
    #define CPP20 true
    #define CPP23 false
#else
    #define CPP20 false
    #define CPP23 false
#endif

#pragma once

#ifndef $DEV_ENV // for debuggers
#define $DEV_ENV


#ifdef LOCAL_JUDGE
    constexpr bool DEV_ENV = true;
    constexpr bool NO_EXCEPT = false;
#else
    constexpr bool DEV_ENV = false;
    constexpr bool NO_EXCEPT = true;
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


#endif // #ifndef $DEV_ENV

#pragma once

#ifdef LOCAL_JUDGE
    #include <cassert>
    #define dev_assert(...) assert(__VA_ARGS__)
#else
    #define dev_assert(...) ({ ; })
#endif

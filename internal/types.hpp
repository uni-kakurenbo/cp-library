#pragma once

#include <cstdint>

namespace lib {

namespace internal {

using size_t = std::int32_t;

using int128_t = __int128_t;
using uint128_t = __uint128_t;

}

} // namespace lib

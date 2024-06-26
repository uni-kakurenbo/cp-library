#pragma once

#include <cstdint>

namespace uni {


using i16 = std::int16_t;
using u16 = std::uint16_t;

using i32 = std::int32_t;
using u32 = std::uint32_t;

using i64 = std::int64_t;
using u64 = std::uint64_t;

#ifdef __GNUC__

using i128 = __int128_t;
using u128 = __uint128_t;

using f128 = __float128;

#endif

using uint = unsigned;
using ll = long long;
using ull = unsigned long long;
using ld = long double;


} // namespace uni

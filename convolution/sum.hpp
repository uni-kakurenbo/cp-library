#pragma once


#include <type_traits>
#include <concepts>
#include <ranges>


#include "convolution/internal/butterfly.hpp"

#include "numeric/modint.hpp"


namespace lib {


template<std::ranges::range Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires
        std::same_as<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>> &&
        internal::static_modint_family<std::ranges::range_value_t<R0>>
Res convolution(R0&& v0, R1&& v1) {
    using mint = std::ranges::range_value_t<R0>;

    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    if(!n || !m) return {};

    const auto z = to_signed(std::bit_ceil(to_unsigned(n + m - 1)));
    assert((mint::mod() - 1) % z == 0);

    if(lib::min(n, m) <= 60) return internal::convolution_naive<Res>(v0, v1);
    return internal::convolution_fft<Res>(v0, v1);
}


template<
    u32 Mod = 998244353,
    std::ranges::range Res,
    std::ranges::sized_range R0, std::ranges::sized_range R1
>
    requires
        std::convertible_to<std::ranges::range_value_t<R0>, static_modint_32bit<Mod>> &&
        std::convertible_to<std::ranges::range_value_t<R1>, static_modint_32bit<Mod>>
Res convolution(R0&& v0, R1&& v1) {
    using mint = static_modint_32bit<Mod>;

    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    if(!n || !m) return {};

    const auto z = to_signed(std::bit_ceil(to_unsigned(n + m - 1)));
    assert((mint::Mod() - 1) % z == 0);

    std::vector<mint> a2(n), b2(m);

    REP(i, n) a2[i] = mint{ v0[i] };
    REP(i, m) b2[i] = mint{ v1[i] };

    const auto c2 = convolution(a2, b2);

    Res c(n + m - 1);
    REP(i, n + m - 1) c[i] = c2[i].val();

    return c;
}


template<std::ranges::range Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires
        std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>> &&
        std::integral<std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>> &&
        std::convertible_to<
            std::ranges::range_value_t<Res>,
            std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
        > &&
        (
            std::numeric_limits<
                std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
            >::digits <= 64
        )
Res convolution(R0&& v0, R1&& v1) {
    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    if(!n || !m) return {};

    static constexpr u64 MOD1 = 754974721;  // 2^24
    static constexpr u64 MOD2 = 167772161;  // 2^25
    static constexpr u64 MOD3 = 469762049;  // 2^26

    static constexpr u64 M2M3 = MOD2 * MOD3;
    static constexpr u64 M1M3 = MOD1 * MOD3;
    static constexpr u64 M1M2 = MOD1 * MOD2;

    static constexpr u64 M1M2M3 = MOD1 * MOD2 * MOD3;

    static constexpr u64 i1 = atcoder::internal::inv_gcd(MOD2 * MOD3, MOD1).second;
    static constexpr u64 i2 = atcoder::internal::inv_gcd(MOD1 * MOD3, MOD2).second;
    static constexpr u64 i3 = atcoder::internal::inv_gcd(MOD1 * MOD2, MOD3).second;

    static constexpr size_t MAX_AB_BIT = 24;

    static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1, "MOD1 isn't enough to support an array length of 2^24.");
    static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1, "MOD2 isn't enough to support an array length of 2^24.");
    static_assert(MOD3 % (1ull << MAX_AB_BIT) == 1, "MOD3 isn't enough to support an array length of 2^24.");

    assert(n + m - 1 <= (1 << MAX_AB_BIT));

    const auto c1 = convolution<MOD1>(v0, v1);
    const auto c2 = convolution<MOD2>(v0, v1);
    const auto c3 = convolution<MOD3>(v0, v1);

    Res c(n + m - 1);
    REP(i, n + m - 1) {
        u64 x = 0;
        x += (c1[i] * i1) % MOD1 * M2M3;
        x += (c2[i] * i2) % MOD2 * M1M3;
        x += (c3[i] * i3) % MOD3 * M1M2;

        const auto diff = c1[i] - lib::mod(to_signed(x), to_signed(MOD1));
        if(diff < 0) diff += MOD1;

        static constexpr u64 offset[5] = { 0, 0, M1M2M3, 2 * M1M2M3, 3 * M1M2M3 };

        x -= offset[diff % 5];
        c[i] = x;
    }

    return c;
}


template<std::ranges::range R0, std::ranges::range R1>
    requires
        std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
auto convolution(R0&& v0, R1&& v1) {
    using common_type = std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>;
    return convolution<valarray<common_type>>(v0, v1);
}


template<u32 Mod = 998244353, std::ranges::range R0, std::ranges::range R1>
    requires
        std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
auto convolution(R0&& v0, R1&& v1) {
    using common_type = std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>;
    return convolution<Mod, valarray<common_type>>(v0, v1);
}


} // namespace lib

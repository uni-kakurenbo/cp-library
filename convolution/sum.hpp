#pragma once


#include <type_traits>
#include <concepts>
#include <ranges>


#include "convolution/internal/butterfly.hpp"

#include "numeric/modular/modint.hpp"


namespace lib {


template<std::ranges::range Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires
        std::same_as<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>> &&
        internal::static_modint_family<std::ranges::range_value_t<R0>> &&
        std::convertible_to<std::ranges::range_value_t<R0>, std::ranges::range_value_t<Res>>
Res convolution(R0&& v0, R1&& v1) {
    using mint = std::ranges::range_value_t<R0>;

    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    if(!n || !m) return {};

    const auto z = to_signed(std::bit_ceil(to_unsigned(n + m - 1)));
    assert((mint::mod() - 1) % z == 0);

    if(lib::min(n, m) <= 60) return convolution_naive<Res>(v0, v1);
    return convolution_fft<Res>(v0, v1);
}


template<
    u32 Mod = 998244353,
    std::ranges::range Res,
    std::ranges::sized_range R0, std::ranges::sized_range R1
>
    requires
        std::convertible_to<std::ranges::range_value_t<R0>, static_modint_32bit<Mod>> &&
        std::convertible_to<std::ranges::range_value_t<R1>, static_modint_32bit<Mod>> &&
        std::convertible_to<std::ranges::range_value_t<R0>, std::ranges::range_value_t<Res>>
std::remove_reference_t<Res> convolution(R0&& v0, R1&& v1) {
    using mint = static_modint_32bit<Mod>;

    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    if(!n || !m) return {};

    const auto z = to_signed(std::bit_ceil(to_unsigned(n + m - 1)));
    assert((mint::mod() - 1) % z == 0);

    std::vector<mint> a2(n), b2(m);

    REP(i, n) a2[i] = mint{ v0[i] };
    REP(i, m) b2[i] = mint{ v1[i] };

    const auto c1 = convolution(a2, b2);

    std::remove_reference_t<Res> c(n + m - 1);
    REP(i, n + m - 1) c[i] = c1[i].val();

    return c;
}


template<std::ranges::range Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires
        std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>> &&
        std::integral<std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>> &&
        (
            std::integral<std::ranges::range_value_t<Res>> ||
            internal::modint_family<std::ranges::range_value_t<Res>>
        ) &&
        std::convertible_to<std::ranges::range_value_t<R0>, std::ranges::range_value_t<Res>>
Res convolution(R0&& v0, R1&& v1) {
    using res_value_type = std::ranges::range_value_t<Res>;

    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    if(!n || !m) return {};

    static constexpr i64 MOD0 = 167772161;
    static constexpr i64 MOD1 = 469762049;
    static constexpr i64 MOD2 = 754974721;

    static constexpr auto M0 = res_value_type{ MOD0 };
    static constexpr auto M1 = res_value_type{ MOD1 };
    static constexpr auto M2 = res_value_type{ MOD2 };
    static constexpr auto M0M1 = M0 * M1;
    static constexpr auto M0M2 = M0 * M2;
    static constexpr auto M1M2 = M1 * M2;
    static constexpr auto M0M1M2 = M0 * M1 * M2;

    static constexpr u64 r0 = atcoder::internal::inv_gcd(MOD1 * MOD2, MOD0).second;
    static constexpr u64 r1 = atcoder::internal::inv_gcd(MOD0 * MOD2, MOD1).second;
    static constexpr u64 r2 = atcoder::internal::inv_gcd(MOD0 * MOD1, MOD2).second;

    static constexpr i64 i01 = atcoder::internal::inv_gcd(MOD0, MOD1).second;
    static constexpr i64 i02 = atcoder::internal::inv_gcd(MOD0, MOD2).second;
    static constexpr i64 i12 = atcoder::internal::inv_gcd(MOD1, MOD2).second;
    static constexpr i64 i02i12 = (i02 * i12) % MOD2;

    static constexpr size_t MAX_AB_BIT = 24;

    static_assert(MOD0 % (1ull << MAX_AB_BIT) == 1, "MOD0 isn't enough to support an array length of 2^24.");
    static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1, "MOD1 isn't enough to support an array length of 2^24.");
    static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1, "MOD2 isn't enough to support an array length of 2^24.");

    assert(n + m - 1 <= (1 << MAX_AB_BIT));

    const auto c0 = convolution<MOD0, R0>(v0, v1);
    const auto c1 = convolution<MOD1, R0>(v0, v1);
    const auto c2 = convolution<MOD2, R0>(v0, v1);

    std::remove_reference_t<Res> c(n + m - 1);

    REP(i, n + m - 1) {
        if constexpr(internal::modint_family<res_value_type>) {
            const auto c01 = ((c1[i] + MOD1 - c0[i]) * i01) % MOD1;
            const auto c012 = ((c2[i] + MOD2 - c0[i]) * i02i12 + (MOD2 - c01) * i12) % MOD2;
            c[i] = c0[i] + c01 * M0 + c012 * M0M1;
        }
        else {
            res_value_type x = 0;
            x += (c0[i] * r0) % MOD0 * M1M2;
            x += (c1[i] * r1) % MOD1 * M0M2;
            x += (c2[i] * r2) % MOD2 * M0M1;

            auto diff = c2[i] - lib::mod(to_signed(x), to_signed(MOD2));
            if (diff < 0) diff += MOD2;

            static constexpr res_value_type offset[5] = { 0, 0, M0M1M2, 2 * M0M1M2, 3 * M0M1M2 };
            x -= offset[diff % 5];
            c[i] = x;
        }
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
        std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>> &&
        std::integral<std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>> &&
        (
            std::numeric_limits<
                std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
            >::digits <= 64
        )
auto convolution(R0&& v0, R1&& v1) {
    using common_type = std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>;
    return convolution<Mod, valarray<common_type>>(v0, v1);
}


} // namespace lib

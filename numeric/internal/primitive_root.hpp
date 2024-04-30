#pragma once


#include <ranges>
#include <vector>
#include <algorithm>
#include <type_traits>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"

#include "numeric/internal/divisors.hpp"
#include "numeric/modular/modint_interface.hpp"

#include "random/engine.hpp"


namespace uni {

namespace internal {


// Thanks to: https://37zigen.com/primitive-root/
template<modint_family Small, modint_family Large, modint_family Mint, std::unsigned_integral T>
T primitive_root(const T p) noexcept(NO_EXCEPT) {
    std::vector<T> pows;
    factorize<Small, Large>(p - 1, &pows);
    {
        std::ranges::sort(pows);
        const auto rest = std::ranges::unique(pows);
        pows.erase(ALL(rest));
    }

    ITRR(pow, pows) pow = (p - 1) / pow;

    if constexpr(dynamic_modint_family<Mint>) Mint::set_mod(p);
    assert(Mint::mod() == p);

    static random_engine_64bit rand;
    while(true) {
        const Mint x = rand();
        if(x == Mint::zero) continue;

        bool ok = true;
        ITR(pow, pows) {
            if(x.pow(pow) == Mint::one) {
                ok = false;
                break;
            }
        }
        if(ok) return x.val();
    }

    return 0;
}


// Thanks to: atcoder::internal::primitive_root_constexpr
template<static_modint_family Mint>
constexpr u32 primitive_root_constexpr(u32 m) {
    assert(Mint::mod() == m);

    u32 divs[20]{}; divs[0] = 2;
    u32 cnt = 1;

    u64 x = (m - 1) / 2;
    while(x % 2 == 0) x /= 2;

    for(u64 i = 3; i * i <= x; i += 2) {
        if(x % i == 0) {
            divs[cnt++] = i;
            while(x % i == 0) x /= i;
        }
    }

    if(x > 1) divs[cnt++] = x;

    for(u32 g = 2; ; g++) {
        bool ok = true;

        REP(i, cnt) {
            if(Mint{ g }.pow((m - 1) / divs[i]) == 1) {
                ok = false;
                break;
            }
        }

        if(ok) return g;
    }
}


template<modint_family Small, modint_family Large = Small, std::integral Res = u64>
constexpr Res primitive_root(const u64 p) noexcept(NO_EXCEPT) {
    if(p == 2) return 1;
    if(p == 167772161) return 3;
    if(p == 469762049) return 3;
    if(p == 754974721) return 11;
    if(p == 998244353) return 3;
    if(p == (u64{ 1 } << 61) - 1) return 37;


    if(std::is_constant_evaluated()) {
        if constexpr(static_modint_family<Small> && std::same_as<Small, Large> && Small::mod() < (1U << 31)) {
            assert(p <= std::numeric_limits<u32>::max());
            return primitive_root_constexpr<Small>(p);
        }
        assert(false);
    }
    else {
        if(p <= Small::max()) return primitive_root<Small, Large, Small, typename Small::value_type>(p);
        else return primitive_root<Small, Large, Large, typename Large::value_type>(p);
    }
}


} // namespace internal

} // namespace uni

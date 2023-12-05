#pragma once


#include <ranges>
#include <vector>
#include <algorithm>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"

#include "numeric/internal/divisors.hpp"
#include "numeric/internal/modint_interface.hpp"

#include "random/xorshift.hpp"


namespace lib {

namespace internal {


// Thanks to: https://37zigen.com/primitive-root/
template<modint_family Small, modint_family Large, modint_family Mint, std::unsigned_integral T>
constexpr T primitive_root(const T p) noexcept(NO_EXCEPT) {
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

    xorshift64 rand;
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

    assert(false);
}


template<modint_family Small, modint_family Large, std::integral Res = u64>
constexpr Res primitive_root(const u64 p) noexcept(NO_EXCEPT) {
    if(p == 2) return 1;
    if(p == 167772161) return 3;
    if(p == 469762049) return 3;
    if(p == 754974721) return 11;
    if(p == 998244353) return 3;

    if(p <= Small::max()) return primitive_root<Small, Large, Small, typename Small::unsigned_value_type>(p);
    else return primitive_root<Small, Large, Large, typename Large::unsigned_value_type>(p);
}


} // namespace internal

} // namespace lib

#pragma once

#include <cassert>
#include <utility>
#include <algorithm>
#include <bit>

#include "snippet/internal/types.hpp"
#include "internal/dev_env.hpp"

#include "numeric/internal/modint_interface.hpp"
#include "numeric/bit.hpp"


namespace lib {

namespace internal {

//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {

namespace internal {


// Miller-Rabin primality test
template<modint_family Mint>
constexpr bool primality_test(const u64 n, const std::vector<u64>& ws) noexcept(NO_EXCEPT) {
    assert(n <= Mint::max());

    if constexpr(dynamic_modint_family<Mint>) Mint::set_mod(n);
    assert(Mint::mod() == n);

    const u64 d = (n - 1) >> std::countr_zero(n - 1);

    const Mint zero = 0, one = 1, rev = n - 1;
    for(u64 w : ws) {
        Mint x = w;
        if(x == zero) continue;
        x = x.pow(d);
        if(x == one || x == rev) continue;
        u64 t = d;
        while(t != n-1 && x != one && x != rev) x *= x, t <<= 1;
        if(x != rev) return false;
    }

    return true;
}


// Thanks to: https://miller-rabin.appspot.com/
template<modint_family Small, modint_family Large = Small>
inline constexpr bool is_prime(const u64 n) noexcept(NO_EXCEPT) {
    if(n == 0 || n == 1) return false;
    if(~n & 1UL) return n == 2UL;

    auto bases = [&]() constexpr noexcept(NO_EXCEPT) -> vector<u64> {
        if(n < 341531UL) return { 9345883071009581737UL };
        else if(n < 1050535501UL) return { 336781006125UL, 9639812373923155UL };
        else if(n < 350269456337UL) return { 4230279247111683200UL, 14694767155120705706UL, 16641139526367750375UL };
        else if(n < 55245642489451UL) return { 2UL, 141889084524735UL, 1199124725622454117UL, 11096072698276303650UL };
        else if(n < 7999252175582851UL) return { 2UL, 4130806001517UL, 149795463772692060UL, 186635894390467037UL, 3967304179347715805UL };
        else if(n < 585226005592931977UL) return { 2UL, 123635709730000UL, 9233062284813009UL, 43835965440333360UL, 761179012939631437UL, 1263739024124850375UL };
        else return { 2UL, 325UL, 9375UL, 28178UL, 450775UL, 9780504UL, 1795265022UL };
    };

    if(n <= Small::max()) return primality_test<Small>(n, bases());
    else return primality_test<Large>(n, bases());
}


constexpr i64 INTERNAL_MODINT_ID = -(1UL << 62);


inline constexpr bool is_prime(const u64 n) noexcept(NO_EXCEPT) {
    return is_prime<dynamic_modint_32bit<INTERNAL_MODINT_ID>, dynamic_modint_64bit<INTERNAL_MODINT_ID>>(n);
}


template<u64 N>
inline constexpr bool is_prime() noexcept(NO_EXCEPT) {
    return is_prime<static_modint_32bit<N>, static_modint_64bit<N>>(N);
}


} // namespace internal

} // namespace fast_factorize_impl


using fast_factorize_impl::internal::is_prime;


} // namespace internal

} // namespace lib

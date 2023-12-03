#pragma once

#include <cassert>
#include <utility>
#include <algorithm>


#include "snippet/internal/types.hpp"
#include "internal/dev_env.hpp"

#include "numeric/internal/modint_interface.hpp"


namespace lib {

namespace internal {


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {

namespace internal {


// Miller-Rabin primality test
template<modint_family Mint>
constexpr bool primality_test(const u64 n, const std::initializer_list<u64> as) noexcept(NO_EXCEPT) {
    assert(n <= Mint::max());

    if constexpr(dynamic_modint_family<Mint>) Mint::set_mod(n);
    assert(Mint::mod() == n);

    u64 d = n - 1;

    while(~d & 1) d >>= 1;

    Mint e = 1, rev = n - 1;
    for(u64 a : as) {
        if(n <= a) break;
        u64 t = d;
        Mint y = Mint(a).pow(t);
        while(t != n - 1 && y != e && y != rev) y *= y, t *= 2;
        if(y != rev && t % 2 == 0) return false;
    }

    return true;
}


template<modint_family Mint32, modint_family Mint64>
inline constexpr bool is_prime(const u64 n) noexcept(NO_EXCEPT) {
    if(~n & 1) return n == 2;
    if(n <= 1) return false;

    if(n <= Mint32::max()) return primality_test<Mint32>(n, { 2, 7, 61 });
    else return primality_test<Mint64>(n, { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 });
}


} // namespace internal

} // namespace fast_factorize_impl


using fast_factorize_impl::internal::is_prime;


} // namespace internal

} // namespace lib

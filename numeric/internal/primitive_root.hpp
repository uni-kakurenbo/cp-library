#pragma once


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"

#include "numeric/internal/divisors.hpp"
#include "numeric/internal/modint_interface.hpp"


namespace lib {

namespace internal {


template<modint_family Mint>
constexpr u64 primitive_root(const u64 p) noexcept(NO_EXCEPT) {
    const auto divs = divisors<Mint>(p - 1);

    if constexpr(dynamic_modint_family<Mint>) Mint::set_mod(p);
    assert(Mint::mod() == p);

    const Mint one = Mint::raw(1), rev = Mint::raw(p - 1);
    REP(x, 1, p) {
        bool ok = true;
        ITR(div, divs) {
            const Mint r = Mint{ x }.pow(div);
	        if (r != one && r != rev) {
		    ok = false;
		    break;
		}
        }
	if(ok) return x;
    }

    assert(false);
}

template<modint_family Small, modint_family Large>
constexpr u64 primitive_root(const u64 p) noexcept(NO_EXCEPT) {
    if(p <= Small::max()) return primitive_root<Small>(p);
    else return primitive_root<Large>(p);
}


} // namespace internal

} // namespace lib

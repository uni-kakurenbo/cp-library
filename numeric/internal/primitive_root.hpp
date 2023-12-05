#pragma once


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"

#include "numeric/internal/divisors.hpp"
#include "numeric/internal/modint_interface.hpp"


namespace lib {

namespace internal {


template<modint_family Mint>
constexpr u64 primitive_root(const u64 p) noexcept(NO_EXCEPT) {
  if constexpr(dynamic_modint_family<Mint>) Mint::set_mod(p);
  assert(Mint::mod() == p);
  
	const auto divs = divisor(p - 1);

  const Mint one = Mint::raw(1), rev = Mint::raw(p - 1);
  REP(x, 1, p) {
		bool ok = true;
    ITR(div, divs) {
      const Mint r = Mint{ i }.pow(div);
			if (r == one && v != rev) {
				ok = false;
				break;
			}
		}
		if(ok) return i;
	}

  assert(false);
}


} // namespace internal

} // namespace lib

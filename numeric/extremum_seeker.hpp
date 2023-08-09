#pragma once


#include <functional>

#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/limits.hpp"


namespace lib {


template<class Res, class Arg>
struct extremum_seeker {
  protected:
    std::function<Res(Arg)> func;

  public:
    extremum_seeker(std::function<Res(Arg)> func) noexcept(NO_EXCEPT) : func(func) {}

    template<const internal::size_t ITERATIONS = 100'000>
    std::pair<Arg,Arg> arg_min(
        const Arg _low = lib::numeric_limits<Arg>::arithmetic_negative_infinity(),
        const Arg _high = lib::numeric_limits<Arg>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        Arg low = _low, high = _high;
        REP(ITERATIONS) {
            const Arg p = low + (high - low) / 3;
            const Arg q = high - (high - low) / 3;
            if(this->func(p) < this->func(q)) {
                high = q;
            }
            else {
                low = p;
            }
        }

        return { low, high };
    }

    template<const internal::size_t ITERATIONS = 100'000>
    std::pair<Arg,Arg> arg_max(
        const Arg _low = lib::numeric_limits<Arg>::arithmetic_negative_infinity(),
        const Arg _high = lib::numeric_limits<Arg>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        Arg low = _low, high = _high;
        REP(ITERATIONS) {
            const Arg p = low + (high - low) / 3;
            const Arg q = high - (high - low) / 3;
            if(this->func(p) > this->func(q)) {
                high = q;
            }
            else {
                low = p;
            }
        }

        return { low, high };
    }


    template<const internal::size_t ITERATIONS = 100'000>
    Res min(
        const Arg _low = lib::numeric_limits<Arg>::arithmetic_negative_infinity(),
        const Arg _high = lib::numeric_limits<Arg>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        auto [ low, high ] = this->arg_min<ITERATIONS>(_low, _high);
        Res res = std::min(this->func(low), this->func(high));
        FOR(x, low, high) chmin(res, this->func(x));
        return res;
    }

    template<const internal::size_t ITERATIONS = 100'000>
    Res max(
        const Arg _low = lib::numeric_limits<Arg>::arithmetic_negative_infinity(),
        const Arg _high = lib::numeric_limits<Arg>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        auto [ low, high ] = this->arg_max<ITERATIONS>(_low, _high);
        Res res = std::max(this->func(low), this->func(high));
        FOR(x, low, high) chmax(res, this->func(x));
        return res;
    }
};


} // namespace lib

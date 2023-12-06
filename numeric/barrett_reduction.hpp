#pragma once


#include <utility>
#include <limits>
#include <type_traits>


#include "internal/types.hpp"
#include "snippet/aliases.hpp"


namespace lib {

namespace internal {


//Thanks to: https://nyaannyaan.github.io/library/modint/barrett-reduction.hpp
template<class T, class R = T>
    requires (std::numeric_limits<R>::digits > 30)
struct barrett_32bit {
    using value_type = T;
    using mod_type = R;

  private:
    value_type _mod;
    u64 _m;

    constexpr inline std::pair<u64,u32> _reduce(const value_type v) const noexcept(NO_EXCEPT) {
        const u64 x = static_cast<u64>((static_cast<u128>(v) * this->_m) >> 64);
        return { x, static_cast<u32>(v - x * this->_mod) };
    }

  public:
    constexpr barrett_32bit() noexcept = default;
    constexpr explicit inline barrett_32bit(const mod_type m) : _mod(m), _m(static_cast<u64>(-1) / m + 1) {}

    constexpr inline value_type quotient(const value_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<value_type>(this->_mod <= r ? x - 1 : x);
    }

    constexpr inline mod_type remainder(const value_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<mod_type>(this->_mod <= r ? r + this->_mod : r);
    }

    constexpr inline std::pair<value_type,mod_type> divide(const value_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        if(this->_mod <= r) return { static_cast<value_type>(x - 1), static_cast<mod_type>(r + this->_mod) };
        return { static_cast<value_type>(x), static_cast<mod_type>(r) };
    }

    constexpr inline mod_type pow(const value_type v, value_type p) const noexcept(NO_EXCEPT) {
        value_type d = this->remainder(v);
        mod_type r = this->_mod == 1 ? 0 : 1;

        while(p > 0) {
            if(p & 1) r = this->remainder(static_cast<value_type>(r) * d);
            d = this->remainder(d * d);
            p >>= 1;
        }

        return r;
    }
};


} // namespace internal


using internal::barrett_32bit;


} // namespace lib

#pragma once


#include <utility>
#include <limits>
#include <type_traits>


#include "internal/types.hpp"
#include "snippet/aliases.hpp"


namespace lib {

namespace internal {

//Thanks to: https://nyaannyaan.github.io/library/modint/barrett-reduction.hpp
struct barrett_32bit {
  private:
    u64 _mod = -1;
    u64 _m;

    constexpr inline std::pair<u64,u32> _reduce(const u64 v) const noexcept(NO_EXCEPT) {
        const u64 x = static_cast<u64>((static_cast<u128>(v) * this->_m) >> 64);
        return { x, static_cast<u32>(v - x * this->_mod) };
    }

  public:
    constexpr barrett_32bit() noexcept = default;
    constexpr explicit inline barrett_32bit(const u32 m) : _mod(m), _m(std::numeric_limits<u64>::max() / m + 1) {
        assert(0 < m && m < std::numeric_limits<u32>::max());
    }

    constexpr inline u64 mod() const noexcept(NO_EXCEPT) { return this->_mod; }

    constexpr inline u64 quotient(const u64 v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<u64>(this->_mod <= r ? x - 1 : x);
    }

    constexpr inline u32 remainder(const u64 v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<u32>(this->_mod <= r ? r + this->_mod : r);
    }

    constexpr inline std::pair<u64,u32> divide(const u64 v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        if(this->_mod <= r) return { static_cast<u64>(x - 1), static_cast<u32>(r + this->_mod) };
        return { static_cast<u64>(x), static_cast<u32>(r) };
    }

    constexpr inline u32 multiply(const u64 x, const u64 y) const noexcept(NO_EXCEPT) {
        return this->remainder(x * y);
    }

    constexpr inline u32 pow(const u64 v, u64 p) const noexcept(NO_EXCEPT) {
        u64 d = this->remainder(v);

        if(this->_mod == 1) return 0;
        if(p == 0) return 1;
        if(p == 1 || d == 0 || d == 1) return d;

        u32 r = 1;

        while(p > 0) {
            if(p & 1) r = this->remainder(static_cast<u64>(r) * d);
            d = this->remainder(static_cast<u64>(d) * d);
            p >>= 1;
        }

        return r;
    }
};


} // namespace internal


using internal::barrett_32bit;


} // namespace lib

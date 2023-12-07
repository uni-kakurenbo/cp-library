#pragma once


#include <utility>
#include <limits>
#include <type_traits>


#include "snippet/aliases.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"
#include "numeric/bit.hpp"


namespace lib {

namespace internal {


template<std::unsigned_integral Value, std::unsigned_integral Large>
    requires has_double_digits_of<Large, Value>
struct barrett_context {
    using value_type = Value;
    using large_type = Large;

  private:
    large_type _mod;
    large_type _m;

    constexpr inline std::pair<large_type,value_type> _reduce(const large_type v) const noexcept(NO_EXCEPT) {
        large_type x = multiply_high(v, this->_m);
        return { x, static_cast<value_type>(v - x * this->_mod) };
    }

  public:
    static constexpr int digits = std::numeric_limits<value_type>::digits - 1;
    static constexpr value_type max() noexcept { return (value_type{ 1 } << barrett_context::digits) - 1; }

    constexpr barrett_context() noexcept = default;
    constexpr explicit inline barrett_context(const value_type m)
      : _mod(m), _m(std::numeric_limits<large_type>::max() / m + 1)
    {
        assert(0 < m && m <= barrett_context::max());
    }

    constexpr inline large_type mod() const noexcept(NO_EXCEPT) { return this->_mod; }

    constexpr inline large_type quotient(const large_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<large_type>(this->_mod <= r ? x - 1 : x);
    }

    constexpr inline value_type remainder(const large_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<value_type>(this->_mod <= r ? r + this->_mod : r);
    }

    constexpr inline std::pair<large_type,value_type> divide(const large_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        if(this->_mod <= r) return { static_cast<large_type>(x - 1), static_cast<value_type>(r + this->_mod) };
        return { static_cast<large_type>(x), static_cast<value_type>(r) };
    }

    constexpr inline value_type multiply(const large_type x, const large_type y) const noexcept(NO_EXCEPT) {
        return this->remainder(x * y);
    }

    constexpr inline value_type pow(const large_type v, large_type p) const noexcept(NO_EXCEPT) {
        large_type d = this->remainder(v);

        if(this->_mod == 1) return 0;
        if(p == 0) return 1;
        if(p == 1 || d == 0 || d == 1) return d;

        value_type r = 1;

        while(p > 0) {
            if(p & 1) r = this->remainder(static_cast<large_type>(r) * d);
            d = this->remainder(static_cast<large_type>(d) * d);
            p >>= 1;
        }

        return r;
    }
};


} // namespace internal


using barrett_32bit = internal::barrett_context<u32, u64>;
using barrett_64bit = internal::barrett_context<u64, u128>;


} // namespace lib

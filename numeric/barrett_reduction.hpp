#pragma once


#include <utility>
#include <limits>
#include <type_traits>


#include "snippet/aliases.hpp"

#include "internal/types.hpp"
#include "internal/concepts.hpp"

#include "numeric/bit.hpp"
#include "numeric/arithmetic.hpp"


namespace lib {

namespace internal {


template<std::unsigned_integral Value, std::unsigned_integral Large>
    requires has_double_digits_of<Large, Value>
struct barrett_reduction {
    using value_type = Value;
    using large_type = Large;

  private:
    large_type _mod = 0, _mi;

    inline constexpr std::pair<large_type,value_type> _reduce(const large_type v) const noexcept(NO_EXCEPT) {
        large_type x = multiply_high(v, this->_mi);
        return { x, static_cast<value_type>(v - x * this->_mod) };
    }

  public:
    static constexpr int digits = std::numeric_limits<value_type>::digits - 1;
    static constexpr value_type max() noexcept { return (value_type{ 1 } << barrett_reduction::digits) - 1; }

    inline constexpr value_type mod() const noexcept(NO_EXCEPT) { return this->_mod; }


    constexpr barrett_reduction() noexcept = default;

    constexpr explicit inline barrett_reduction(const value_type mod)
      : _mod(mod), _mi(std::numeric_limits<large_type>::max() / mod + 1)
    {
        assert(0 < mod && mod <= barrett_reduction::max());
    }


    inline constexpr large_type quotient(const large_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<large_type>(this->_mod <= r ? x - 1 : x);
    }

    inline constexpr value_type reduce(const large_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        return static_cast<value_type>(this->_mod <= r ? r + this->_mod : r);
    }

    inline constexpr std::pair<large_type,value_type> divide(const large_type v) const noexcept(NO_EXCEPT) {
        const auto [ x, r ] = this->_reduce(v);
        if(this->_mod <= r) return { static_cast<large_type>(x - 1), static_cast<value_type>(r + this->_mod) };
        return { static_cast<large_type>(x), static_cast<value_type>(r) };
    }

    inline constexpr value_type multiply(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return this->reduce(static_cast<large_type>(x) * static_cast<large_type>(y));
    }

    inline constexpr value_type pow(const large_type v, i64 p) const noexcept(NO_EXCEPT) {
        if(this->_mod == 1) return 0;
        return lib::pow(
            this->reduce(v), p,
            [&](const value_type x, const value_type y) noexcept(NO_EXCEPT) { return this->multiply(x, y); }
        );
    }

    constexpr value_type convert_raw(const large_type v) const noexcept(NO_EXCEPT) {
        return this->reduce(v);
    }

    template<std::integral T>
    constexpr value_type convert(T v) const noexcept(NO_EXCEPT) {
        using common_type = std::common_type_t<T, value_type>;
        const common_type mod = static_cast<common_type>(this->_mod);

        if(v > 0 && static_cast<common_type>(v) >= mod) {
            if(static_cast<common_type>(v) <= barrett_reduction::max()) v = this->reduce(v);
            else v %= mod;
        }

        if constexpr(std::signed_integral<T>) {
            if(v < 0 && static_cast<common_type>(-v) >= mod) {
                if(static_cast<common_type>(-v) <= barrett_reduction::max()) {
                    v = this->reduce(static_cast<value_type>(-v));
                }
                else {
                    v %= mod;
                }

                if(v != 0) v += mod;
            }
        }

        return static_cast<value_type>(v);
    }

    constexpr value_type revert(value_type v) const noexcept(NO_EXCEPT) { return this->reduce(v); }
};


} // namespace internal


using barrett_reduction_32bit = internal::barrett_reduction<u32, u64>;
using barrett_reduction_64bit = internal::barrett_reduction<u64, u128>;


} // namespace lib

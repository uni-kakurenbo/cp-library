#pragma once


#include <concepts>


#include "snippet/aliases.hpp"

#include "internal/concepts.hpp"

#include "numeric/arithmetic.hpp"


namespace lib {

namespace internal {


template<std::unsigned_integral Value, std::unsigned_integral Large>
    requires has_double_digits_of<Large, Value>
struct builtin_reduction {
    using value_type = Value;
    using large_type = Large;

  private:
    value_type _mod;

  public:
    static constexpr int digits = std::numeric_limits<value_type>::digits;
    static constexpr value_type max() noexcept { return std::numeric_limits<value_type>::max(); }

    inline constexpr value_type mod() const noexcept(NO_EXCEPT) { return this->_mod; }

    constexpr builtin_reduction() noexcept = default;

    constexpr builtin_reduction(const value_type mod) noexcept(NO_EXCEPT) : _mod(mod) {
        assert(0 < mod && mod < builtin_reduction::max());
    }


    inline constexpr value_type reduce(const value_type v) const noexcept(NO_EXCEPT) { return v % this->_mod; }


    inline constexpr value_type add(value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        if(x >= this->_mod - y) x -= this->_mod;
        x += y;
        return x;
    }

    inline constexpr value_type subtract(value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        if(x < y) x += this->_mod;
        x -= y;
        return x;
    }


    inline constexpr value_type multiply(const large_type x, const large_type y) const noexcept(NO_EXCEPT) {
        return static_cast<value_type>((x * y) % this->_mod);
    }

    inline constexpr value_type pow(const value_type v, i64 p) const noexcept(NO_EXCEPT) {
        if(this->_mod == 0) return 0;
        return lib::pow(v, p, [this](const value_type x, const value_type y) { return this->multiply(x, y); });
    }


    inline constexpr bool equal(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return x == y;
    }


    constexpr value_type convert_raw(const large_type v) const noexcept(NO_EXCEPT) { return v; }

    template<std::integral T>
    constexpr value_type convert(T v) const noexcept(NO_EXCEPT) {
        using common_type = std::common_type_t<T, value_type>;
        const common_type mod = static_cast<common_type>(this->_mod);

        if(std::is_constant_evaluated()) {
            v %= mod;
            if(v < 0) v += mod;
        }
        else {
            if(v > 0 && static_cast<common_type>(v) >= mod) {
                v %= mod;
            }

            if constexpr(std::signed_integral<T>) {
                if(v < 0) {
                    if(static_cast<common_type>(-v) <= mod) v += mod;
                    else {
                        v %= mod;
                        if(v != 0) v += mod;
                    }
                }
            }
        }

        return static_cast<value_type>(v);
    }

    constexpr value_type revert(const value_type v) const noexcept(NO_EXCEPT) { return this->_mod == 1 ? 0 : v; }
};


} // namespace internal


using builtin_reduction_32bit = internal::builtin_reduction<u32, u64>;
using builtin_reduction_64bit = internal::builtin_reduction<u64, u128>;


} // namespace lib

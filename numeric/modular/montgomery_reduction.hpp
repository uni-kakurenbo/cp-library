#pragma once


#include <utility>
#include <limits>
#include <type_traits>


#include "template/debug.hpp"

#include "snippet/aliases.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"
#include "numeric/bit.hpp"
#include "numeric/arithmetic.hpp"


namespace lib {

namespace internal {


template<std::unsigned_integral Value, std::unsigned_integral Large>
    requires has_double_digits_of<Large, Value>
struct montgomery_reduction {
    using value_type = Value;
    using large_type = Large;

  private:
    value_type _mod = 0, _r2, _mp;

    constexpr value_type _inv() const noexcept(NO_EXCEPT) {
        value_type res = this->_mod;
        while(this->_mod * res != 1) res *= value_type{ 2 } - this->_mod * res;
        return res;
    }

  public:

    static constexpr int digits = std::numeric_limits<value_type>::digits - 2;
    static inline constexpr value_type max() noexcept { return (value_type{ 1 } << montgomery_reduction::digits) - 1; }

    inline constexpr value_type mod() const noexcept(NO_EXCEPT) { return this->_mod; }


    value_type zero = 0;
    value_type one;


    constexpr montgomery_reduction() noexcept = default;

    constexpr montgomery_reduction(const value_type mod) noexcept(NO_EXCEPT)
      : _mod(mod), _r2(static_cast<value_type>(-static_cast<large_type>(mod) % mod)),
        _mp(-this->_inv()), one(this->reduce(this->_r))
    {
        assert((mod & 1) == 1);
        assert(mod <= montgomery_reduction::max());
    }


    constexpr value_type reduce(const large_type v) const noexcept(NO_EXCEPT) {
        return
            static_cast<value_type>(
                (
                    v + static_cast<large_type>(static_cast<value_type>(v) * this->_mp) * this->_mod
                ) >> std::numeric_limits<value_type>::digits
            );
    }


    inline constexpr value_type add(value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        x += y;
        if(x >= (this->_mod << 1)) x -= (this->_mod << 1);
        return x;
    }

    inline constexpr value_type subtract(value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        if(x < y) x += (this->_mod << 1);
        x -= y;
        return x;
    }


    inline constexpr value_type multiply(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return this->reduce(static_cast<large_type>(x) * static_cast<large_type>(y));
    }

    inline constexpr value_type pow(const large_type v, i64 p) const noexcept(NO_EXCEPT) {
        if(this->_mod == 1) return 0;
        return lib::pow(
            v, p,
            [&](const value_type x, const value_type y) noexcept(NO_EXCEPT) { return this->multiply(x, y); },
            static_cast<large_type>(this->one)
        );
    }


    inline constexpr value_type normalize(const value_type v) const noexcept(NO_EXCEPT) {
        assert(0 <= v && v < (this->_mod << 1));
        if(v < this->_mod) return v;
        return v - this->_mod;
    }

    inline constexpr bool equal(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return this->normalize(x) == this->normalize(y);
    }


    inline constexpr value_type convert_raw(const value_type v) const noexcept(NO_EXCEPT) {
        if(v == 1) return this->one;
        return this->multiply(v, this->_r2);
    }

    template<std::integral T>
    constexpr value_type convert(T v) const noexcept(NO_EXCEPT) {
        if(v == 1) return this->one;

        using common_type = std::common_type_t<T, value_type>;
        const common_type mod2 = static_cast<common_type>(this->_mod << 1);

        if(v > 0 && static_cast<common_type>(v) >= mod2) {
            v %= mod2;
        }

        if constexpr(std::is_signed_v<T>) {
            if(v < 0 && static_cast<common_type>(-v) >= mod2) {
                v %= mod2;
                if(v != 0) v += mod2;
            }
        }

        return this->multiply(v, this->_r2);
    }


    constexpr value_type revert(const value_type v) const noexcept(NO_EXCEPT) {
        return this->normalize(this->reduce(v));
    }
};

// Thanks to: https://www.mathenachia.blog/even-mod-montgomery-impl/
template<std::unsigned_integral Value, std::unsigned_integral Large>
    requires has_double_digits_of<Large, Value>
struct arbitrary_montgomery_reduction {
    using value_type = Value;
    using large_type = Large;

  private:
    using context = arbitrary_montgomery_reduction;
    static constexpr int width = std::numeric_limits<value_type>::digits;

    value_type _mod = 0;
    int _tz;
    value_type _m0;
    large_type _m0i, _mask;
    value_type _r2;

    constexpr large_type _inv() const noexcept(NO_EXCEPT) {
        large_type res = this->_m0;
        while(((this->_m0 * res) & this->_mask) != 1) res *= large_type{ 2 } - this->_m0 * res;
        return res & this->_mask;
    }

    constexpr value_type _m0ip() const noexcept(NO_EXCEPT) {
        if(this->_tz == 0) return 0;
        value_type res = this->_m0;
        const value_type mask = (value_type{ 1 } << this->_tz) - 1;
        while(((this->_m0 * res) & mask) != 1) res *= value_type{ 2 } - this->_m0 * res;
        return res & mask;
    }

  public:
    static constexpr int digits = std::numeric_limits<value_type>::digits - 2;
    static inline constexpr value_type max() noexcept { return (value_type{ 1 } << context::digits) - 1; }

    inline constexpr value_type mod() const noexcept(NO_EXCEPT) { return this->_mod; }

    value_type one;


    constexpr arbitrary_montgomery_reduction() noexcept = default;

    constexpr arbitrary_montgomery_reduction(value_type m) noexcept(NO_EXCEPT) {
        assert(0 < m);

        if(this->_mod == m) return;

        this->_mod = m;
        this->_tz = std::countr_zero(m);
        this->_m0 = m >> this->_tz;

        assert(this->_mod < context::max());

        this->_mask = (large_type{ 1 } << (context::width + this->_tz)) - 1;
        this->_m0i = this->_inv();

        {
            const value_type x = (std::numeric_limits<large_type>::max() % this->_m0) + 1;
            const value_type mask = (value_type{ 1 } << this->_tz) - 1;
            this->_r2 = (x + ((((large_type{ 1 } - x) * this->_m0ip()) & mask) * this->_m0));
        }

        this->one = this->reduce(this->_r2);
    }


    constexpr value_type reduce(const large_type v) const noexcept(NO_EXCEPT) {
        const value_type res =
            static_cast<value_type>(
                (
                    v +
                    this->_m0 *
                    ((((v << std::numeric_limits<value_type>::digits) - v) * this->_m0i) & this->_mask)
                ) >> std::numeric_limits<value_type>::digits
            );
        return res;
    }


    inline constexpr value_type add(value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        x += y;
        if(x >= (this->_mod << 1)) x -= (this->_mod << 1);
        return x;
    }

    inline constexpr value_type subtract(value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        if(x < y) x += (this->_mod << 1);
        x -= y;
        return x;
    }



    inline constexpr value_type multiply(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return this->reduce(static_cast<large_type>(x) * static_cast<large_type>(y));
    }

    inline constexpr value_type pow(const large_type v, i64 p) const noexcept(NO_EXCEPT) {
        if(this->_mod == 1) return 0;
        return lib::pow(
            v, p,
            [&](const value_type x, const value_type y) noexcept(NO_EXCEPT) { return this->multiply(x, y); },
            static_cast<large_type>(this->one)
        );
    }

    inline constexpr value_type normalize(const value_type v) const noexcept(NO_EXCEPT) {
        assert(0 <= v && v < (this->_mod << 1));
        if(v < this->_mod) return v;
        return v - this->_mod;
    }

    inline constexpr bool equal(const large_type x, const large_type y) const noexcept(NO_EXCEPT) {
        return this->normalize(x) == this->normalize(y);
    }


    inline constexpr value_type convert_raw(const value_type v) const noexcept(NO_EXCEPT) {
        if(v == 1) return this->one;
        return this->multiply(v, this->_r2);
    }

    template<std::integral T>
    constexpr value_type convert(T v) const noexcept(NO_EXCEPT) {
        if(v == 1) return this->one;

        using common_type = std::common_type_t<T, value_type>;
        const common_type mod2 = static_cast<common_type>(this->_mod << 1);

        if(v > 0 && static_cast<common_type>(v) >= mod2) {
            v %= mod2;
        }

        if constexpr(std::signed_integral<T>) {
            if(v < 0) {
                if(static_cast<common_type>(-v) >= mod2) v %= mod2;
                if(v < 0) v += mod2;
            }
        }

        return this->multiply(v, this->_r2);
    }


    constexpr value_type revert(const value_type v) const noexcept(NO_EXCEPT) {
        return this->normalize(this->reduce(v));
    }
};


} // namespace internal


using montgomery_reduction_32bit = internal::montgomery_reduction<u32, u64>;
using montgomery_reduction_64bit = internal::montgomery_reduction<u64, u128>;

using arbitrary_montgomery_reduction_32bit = internal::arbitrary_montgomery_reduction<u32, u64>;
using arbitrary_montgomery_reduction_64bit = internal::arbitrary_montgomery_reduction<u64, u128>;

} // namespace lib

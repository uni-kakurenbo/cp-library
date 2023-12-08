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
struct montgomery_context {
    using value_type = Value;
    using large_type = Large;

  private:
    value_type _mod = 0, _np, _r2;

    constexpr value_type _mni() const noexcept(NO_EXCEPT) {
        value_type res = this->_mod;
        while(this->_mod * res != 1) res *= value_type{ 2 } - this->_mod * res;
        return res;
    }

  public:
    static constexpr int digits = std::numeric_limits<value_type>::digits - 2;
    static inline constexpr value_type max() noexcept { return (value_type{ 1 } << montgomery_context::digits) - 1; }

    inline constexpr value_type mod() const noexcept(NO_EXCEPT) { return this->_mod; }
    inline constexpr value_type np() const noexcept(NO_EXCEPT) { return this->_np; }
    inline constexpr value_type r2() const noexcept(NO_EXCEPT) { return this->_r2; }


    constexpr montgomery_context() noexcept = default;

    constexpr montgomery_context(const value_type m) noexcept(NO_EXCEPT) {
        assert((m & 1) == 1);

        if(this->_mod == m) return;

        assert(m <= montgomery_context::max());

        this->_mod = m;
        this->_r2 = static_cast<value_type>(-static_cast<large_type>(m) % m);
        this->_np = -this->_mni();
    }


    constexpr value_type reduce(const large_type v) const noexcept(NO_EXCEPT) {
        return
            static_cast<value_type>(
                (
                    v + static_cast<large_type>(static_cast<value_type>(v) * this->_np) * this->_mod
                ) >> std::numeric_limits<value_type>::digits
            );
    }


    inline constexpr value_type multiply(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return this->reduce(static_cast<large_type>(x) * static_cast<large_type>(y));
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
        return this->multiply(v, this->_r2);
    }

    template<std::integral T>
    constexpr value_type convert(T v) const noexcept(NO_EXCEPT) {
        using common_type = std::common_type_t<T, value_type>;
        const common_type mod2 = static_cast<common_type>(this->_mod << 1);

        if(static_cast<common_type>(v) >= mod2) {
            v %= mod2;
        }

        if constexpr(std::is_signed_v<T>) {
            if(v < 0) {
                if(static_cast<common_type>(-v) >= mod2) v %= mod2;
                if(v != 0) v += mod2;
            }
        }

        return this->multiply(v, this->_r2);
    }
};


} // namespace internal


using montgomery_32bit = internal::montgomery_context<u32, u64>;
using montgomery_64bit = internal::montgomery_context<u64, u128>;


} // namespace lib

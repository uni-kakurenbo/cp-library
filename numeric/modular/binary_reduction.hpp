#pragma once


#include <utility>
#include <limits>
#include <type_traits>


#include "snippet/aliases.hpp"

#include "internal/types.hpp"
#include "internal/concepts.hpp"

#include "numeric/bit.hpp"
#include "numeric/arithmetic.hpp"


namespace uni {

namespace internal {


template<std::unsigned_integral Value>
struct binary_reduction {
    using value_type = Value;

  private:
    value_type _mask;

  public:
    static constexpr int digits = std::numeric_limits<value_type>::digits;
    static constexpr value_type max() noexcept { return std::numeric_limits<value_type>::max(); }

    inline constexpr value_type mod() const noexcept(NO_EXCEPT) { return this->_mask + 1; }

    constexpr binary_reduction() noexcept = default;

    constexpr explicit inline binary_reduction(const value_type mod) noexcept(NO_EXCEPT) : _mask(mod - 1) {
        assert(std::has_single_bit(mod));
    }

    inline constexpr value_type reduce(const value_type v) const noexcept(NO_EXCEPT) { return v; }


    inline constexpr value_type add(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return x + y;
    }

    inline constexpr value_type subtract(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return x - y;
    }


    inline constexpr value_type multiply(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return x * y;
    }

    template<std::integral K>
    inline constexpr value_type pow(const value_type v, const K p) const noexcept(NO_EXCEPT) {
        if constexpr(std::signed_integral<K>) assert(p >= 0);

        if(this->_mask == 0) return 0;
        return uni::pow(v, p);
    }


    inline constexpr auto compare(const value_type x, const value_type y) const noexcept(NO_EXCEPT) {
        return this->revert(x) <=> this->revert(y);
    }


    constexpr value_type convert_raw(const value_type v) const noexcept(NO_EXCEPT) {
        return v;
    }

    template<std::integral T>
    constexpr value_type convert(T v) const noexcept(NO_EXCEPT) {
        return static_cast<value_type>(v);
    }


    constexpr value_type revert(const value_type v) const noexcept(NO_EXCEPT) {
        return v & this->_mask;
    }
};


} // namespace internal


using binary_reduction_32bit = internal::binary_reduction<u32>;
using binary_reduction_64bit = internal::binary_reduction<u64>;
using binary_reduction_128bit = internal::binary_reduction<u128>;


} // namespace uni

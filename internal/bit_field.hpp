#pragma once


#include <type_traits>

#include "internal/dev_env.hpp"


namespace lib {

namespace internal {

namespace bit_field {


template<class Flag> struct base {
    using flag = Flag;
    using bit_type = typename std::underlying_type_t<flag>;

  private:
    const bit_type _bits;

  public:
    constexpr bit_type bits() const noexcept(NO_EXCEPT) { return this->_bits; }


    template<class... Flags>
    constexpr base(Flags... flags) : _bits(set(flags...)) {}

    constexpr bool none() const noexcept(NO_EXCEPT) { return this->_bits == 0; }

    template<class... Tail>
    static constexpr bit_type set(flag head, Tail... tail) noexcept(NO_EXCEPT) {
        return static_cast<bit_type>(head) | set(tail...);
    }

    static constexpr bit_type set() noexcept(NO_EXCEPT) { return 0; }

    template<class... Tail> constexpr bool has(flag head, Tail... tail) const noexcept(NO_EXCEPT) {
        return (this->bits() & static_cast<bit_type>(head)) and has(tail...);
    }
    constexpr bool has() const noexcept(NO_EXCEPT) { return true; }

    constexpr bool has(const base& other) const noexcept(NO_EXCEPT) {
        return this->_bits & other._bits == other._bits;
    }
};


} // namespace bit_field

} // namespace internal

} // namespace lib

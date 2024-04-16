#pragma once


#include <type_traits>

#include "internal/dev_env.hpp"


namespace uni {

namespace internal {

namespace bit_field {


template<class Flag> struct base {
    using flag = Flag;
    using bit_type = typename std::underlying_type_t<flag>;

    const bit_type bits;


    template<class... Flags>
    constexpr base(Flags... flags) : bits(set(flags...)) {}


    constexpr bool none() const noexcept(NO_EXCEPT) { return this->bits == 0; }


    template<class... Tail>
    static constexpr bit_type toggle(flag head, Tail... tail) noexcept(NO_EXCEPT) {
        return static_cast<bit_type>(head) ^ toggle(tail...);
    }

    static constexpr bit_type toggle() noexcept(NO_EXCEPT) { return 0; }


    template<class... Tail>
    static constexpr bit_type set(flag head, Tail... tail) noexcept(NO_EXCEPT) {
        return static_cast<bit_type>(head) | set(tail...);
    }

    static constexpr bit_type set() noexcept(NO_EXCEPT) { return 0; }


    template<class... Tail> constexpr bool has(flag head, Tail... tail) const noexcept(NO_EXCEPT) {
        return (this->bits & static_cast<bit_type>(head)) and has(tail...);
    }
    constexpr bool has() const noexcept(NO_EXCEPT) { return true; }

    constexpr bool has(const base& other) const noexcept(NO_EXCEPT) {
        return this->bits & other.bits == other.bits;
    }


    template<class... Flags> constexpr bool equals(Flags... flags) const noexcept(NO_EXCEPT) {
        return this->equals(base{ flags... });
    }
    constexpr bool equals() const noexcept(NO_EXCEPT) { return true; }

    constexpr bool equals(const base& other) const noexcept(NO_EXCEPT) {
        return this->bits == other.bits;
    }
};


} // namespace bit_field

} // namespace internal

} // namespace uni

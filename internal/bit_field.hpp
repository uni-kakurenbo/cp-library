#pragma once

#include <type_traits>

namespace lib {

namespace internal {

namespace bit_field {


template<class Flag> struct base {
    using flag = Flag;
    using bit_type = typename std::underlying_type_t<flag>;

  private:
    const bit_type _bits;

  public:
    constexpr bit_type bits() const { return this->_bits; }


    template<class... Flags>
    constexpr base(Flags... flags) : _bits(set(flags...)) {}


    template<class... Tail>
    static constexpr bit_type set(flag head, Tail... tail) {
        return static_cast<bit_type>(head) | set(tail...);
    }

    static constexpr bit_type set() { return 0; }


    template<class... Tail> constexpr bool has(flag head, Tail... tail) const {
        return (this->bits() & static_cast<bit_type>(head)) and has(tail...);
    }
    constexpr bool has() const { return true; }
};


} // namespace bit_field

} // namespace internal

} // namespace lib
